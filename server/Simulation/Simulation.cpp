#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Simulation.h"

Simulation::Simulation() {
    ///collision configuration contains default setup for memory, collision setup
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a different dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

    m_broadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    //the ground is a cube of side 100 at position y = -56.
    //the sphere will hit it at y = -6, with center at -5
    {
        btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
        m_collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar mass(0.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody *body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        m_dynamicsWorld->addRigidBody(body);

        SimulationObject obj;
        obj.id = 0;
        obj.type = NetworkedPhysics::EntityType_Cube;
        obj.transform = groundTransform;
        obj.scale = btVector3(100, 100, 100);
        m_objects.push_back(obj);
    }

    for (int i = 0; i < 500; i++) {
        createRigidBody(i + 1, randMToN(0.2f, 2.f), btVector3(1, 1, 1), randMToN(1, 1000));
    }


}

Simulation::~Simulation() {
    //remove the rigidbodies from the dynamics world and delete them
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        m_dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < m_collisionShapes.size(); j++) {
        btCollisionShape *shape = m_collisionShapes[j];
        m_collisionShapes[j] = 0;
        delete shape;
    }

    //delete dynamics world
    delete m_dynamicsWorld;

    //delete solver
    delete m_solver;

    //delete broadphase
    delete m_broadphase;

    //delete dispatcher
    delete m_dispatcher;

    delete m_collisionConfiguration;

    //next line is optional: it will be cleared by the destructor when the array goes out of scope
    m_collisionShapes.clear();
}

void Simulation::Update() {
    m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);
}

std::vector<Simulation::SimulationObject> &Simulation::GetSimulationObjects() {
    for (int i = 0; i < m_objects.size(); i++) {

        btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[i];

        btRigidBody *body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(trans);

        } else {
            trans = obj->getWorldTransform();
        }

        m_objects[i].transform = trans;
    }

    return m_objects;
}

void Simulation::ApplyForce(int objectId, const NetworkedPhysics::ClientInputMessage *pInputMessage) {
    btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[objectId];
    btRigidBody *body = btRigidBody::upcast(obj);

    body->activate(true);

    btVector3 direction;
    const int velocity = 10;

    switch (pInputMessage->type()) {
        case NetworkedPhysics::InputType_Forward: {
            direction = btVector3(body->getLinearVelocity().x(), body->getLinearVelocity().y(), velocity);
            break;
        }
        case NetworkedPhysics::InputType_Backward: {
            direction = btVector3(body->getLinearVelocity().x(), body->getLinearVelocity().y(), -velocity);
            break;
        }
        case NetworkedPhysics::InputType_Leftward: {
            direction = btVector3(-velocity, body->getLinearVelocity().y(), body->getLinearVelocity().z());
            break;
        }
        case NetworkedPhysics::InputType_Rightward: {
            direction = btVector3(velocity, body->getLinearVelocity().y(), body->getLinearVelocity().z());
            break;
        }
        default:
            printf("got unsupported input type");
    }

    body->setLinearVelocity(direction);
}

int Simulation::GetNewPlayerObjectId() {
    int id = m_dynamicsWorld->getNumCollisionObjects();

    createRigidBody(id, 50, btVector3(2, 2, 2), -1);

    return id;
}

void Simulation::Reset() {
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 1 /* ignore ground plane */; i--) {
        btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btTransform &trans = obj->getWorldTransform();
        trans.setIdentity();
        trans.setOrigin(btVector3(randMToN(-5, 5), 10 + randMToN(0, 10), randMToN(-5, 5)));
        m_objects[i].transform = trans;

        btRigidBody *body = btRigidBody::upcast(obj);
        body->clearForces();
        body->setLinearVelocity(btVector3(0, 0, 0));
        body->setAngularVelocity(btVector3(0, 0, 0));
    }
}

void Simulation::createRigidBody(int id, float mass, btVector3 scale, float friction) {
    /// create a dynamic rigidbody
    btCollisionShape *colShape = new btBoxShape(btVector3(btScalar(.5f), btScalar(.5), btScalar(.5)));
    colShape->setLocalScaling(scale);
    m_collisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(randMToN(-5, 5), 10 + randMToN(0, 10), randMToN(-5, 5)));

    btScalar rbmass(mass);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (rbmass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        colShape->calculateLocalInertia(rbmass, localInertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(rbmass, myMotionState, colShape, localInertia);
    if (friction > 0)
        rbInfo.m_friction = friction;
    btRigidBody *body = new btRigidBody(rbInfo);

    m_dynamicsWorld->addRigidBody(body);

    SimulationObject obj;
    obj.id = id;
    obj.type = NetworkedPhysics::EntityType_Cube;
    obj.transform = startTransform;
    obj.scale = colShape->getLocalScaling();
    m_objects.push_back(obj);
}
