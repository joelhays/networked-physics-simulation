#ifndef NETWORKEDPHYSICS_SIMULATION_H
#define NETWORKEDPHYSICS_SIMULATION_H


#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <vector>
#include "../Flatbuffer/message_generated.h"

static float randMToN(float M, float N) {
    return M + (rand() / (RAND_MAX / (N - M)));
}

class Simulation {
public:
    struct SimulationObject {
        int id;
        btTransform transform;
        btVector3 scale;
        NetworkedPhysics::EntityType type;

        SimulationObject() {
            id = -1;
        }
    };

    Simulation();

    ~Simulation();

    // Advances the simulation
    void Update();

    // Transforms current state of the physics simulation into something more easily consumed
    // Should be called after Update
    std::vector<SimulationObject> &GetSimulationObjects();

    // Creates a new simulation object and returns the Id
    int GetNewPlayerObjectId();

    // Sets the linear velocity of a simulation object
    // Should be called before Update
    void ApplyForce(int objectId, const NetworkedPhysics::ClientInputMessage *pInputMessage);

    void Reset();

private:
    btDefaultCollisionConfiguration *m_collisionConfiguration;
    btCollisionDispatcher *m_dispatcher;
    btBroadphaseInterface *m_broadphase;
    btSequentialImpulseConstraintSolver *m_solver;
    btDiscreteDynamicsWorld *m_dynamicsWorld;

    btAlignedObjectArray<btCollisionShape *> m_collisionShapes;
    
    std::vector<SimulationObject> m_objects;

private:
    void createRigidBody(int id, float mass, btVector3 scale, float friction);
};


#endif //NETWORKEDPHYSICS_SIMULATION_H
