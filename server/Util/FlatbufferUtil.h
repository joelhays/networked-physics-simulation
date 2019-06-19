#ifndef NETWORKEDPHYSICS_FLATBUFFERUTIL_H
#define NETWORKEDPHYSICS_FLATBUFFERUTIL_H

#include <LinearMath/btVector3.h>
#include "../Flatbuffer/message_generated.h"
#include "../Simulation/Simulation.h"
#include "../Server/Player.h"

namespace FlatbufferUtil {
    flatbuffers::Offset<NetworkedPhysics::Vec3>
    CreatePositionOffset(flatbuffers::FlatBufferBuilder &fbb,
                         const Simulation::SimulationObject &baseline,
                         const Simulation::SimulationObject &simObject) {

        btVector3 positionDelta = baseline.transform.getOrigin() - simObject.transform.getOrigin();

        if (positionDelta.length() > 0.001f) {
            NetworkedPhysics::Vec3Builder positionBuilder(fbb);
            positionBuilder.add_x(simObject.transform.getOrigin().x());
            positionBuilder.add_y(simObject.transform.getOrigin().y());
            positionBuilder.add_z(simObject.transform.getOrigin().z());
            return positionBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::Vec3>();
    }

    flatbuffers::Offset<NetworkedPhysics::Vec4>
    CreateRotationOffset(flatbuffers::FlatBufferBuilder &fbb,
                         const Simulation::SimulationObject &baseline,
                         const Simulation::SimulationObject &simObject) {

        btQuaternion rotationDelta = baseline.transform.getRotation() - simObject.transform.getRotation();
        if (rotationDelta.length() > 0.001f) {
            NetworkedPhysics::Vec4Builder rotationBuilder(fbb);
            rotationBuilder.add_x(simObject.transform.getRotation().x());
            rotationBuilder.add_y(simObject.transform.getRotation().y());
            rotationBuilder.add_z(simObject.transform.getRotation().z());
            rotationBuilder.add_w(simObject.transform.getRotation().w());
            return rotationBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::Vec4>();
    }

    flatbuffers::Offset<NetworkedPhysics::Vec3>
    CreateScaleOffset(flatbuffers::FlatBufferBuilder &fbb,
                      const Simulation::SimulationObject &baseline,
                      const Simulation::SimulationObject &simObject) {

        btVector3 scaleDelta = baseline.scale - simObject.scale;
        if (scaleDelta.length() > 0.001f) {
            NetworkedPhysics::Vec3Builder scaleBuilder(fbb);
            scaleBuilder.add_x(simObject.scale.x());
            scaleBuilder.add_y(simObject.scale.y());
            scaleBuilder.add_z(simObject.scale.z());
            return scaleBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::Vec3>();
    }

    flatbuffers::Offset<NetworkedPhysics::Transform>
    CreateTransformOffset(flatbuffers::FlatBufferBuilder &fbb,
                          const flatbuffers::Offset<NetworkedPhysics::Vec3> &positionOffset,
                          const flatbuffers::Offset<NetworkedPhysics::Vec4> &rotationOffset,
                          const flatbuffers::Offset<NetworkedPhysics::Vec3> &scaleOffset) {

        if (!positionOffset.IsNull() || !rotationOffset.IsNull() || !scaleOffset.IsNull()) {
            NetworkedPhysics::TransformBuilder transformBuilder(fbb);
            if (!positionOffset.IsNull()) transformBuilder.add_position(positionOffset);
            if (!rotationOffset.IsNull()) transformBuilder.add_rotation(rotationOffset);
            if (!scaleOffset.IsNull()) transformBuilder.add_scale(scaleOffset);
            return transformBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::Transform>();
    }

    flatbuffers::Offset<NetworkedPhysics::Entity>
    CreateEntityOffset(flatbuffers::FlatBufferBuilder &fbb,
                       const flatbuffers::Offset<NetworkedPhysics::Transform> &transformOffset,
                       const Simulation::SimulationObject &simObject,
                       const Player &player) {

        if (!transformOffset.IsNull()) {
            NetworkedPhysics::EntityBuilder entityBuilder(fbb);
            entityBuilder.add_interacting(player.objectId == simObject.id);
            entityBuilder.add_transform(transformOffset);
            entityBuilder.add_type(simObject.type);
            entityBuilder.add_id(simObject.id);
            return entityBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::Entity>();
    }

    flatbuffers::Offset<NetworkedPhysics::ServerSnapshotMessage>
    CreateSnapshotOffset(flatbuffers::FlatBufferBuilder &fbb,
                         const flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<NetworkedPhysics::Entity>>> &entitiesVectorOffset) {

        if (!entitiesVectorOffset.IsNull()) {
            NetworkedPhysics::ServerSnapshotMessageBuilder snapshotBuilder(fbb);
            snapshotBuilder.add_entities(entitiesVectorOffset);
            return snapshotBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::ServerSnapshotMessage>();
    }

    flatbuffers::Offset<NetworkedPhysics::Message>
    CreateMessageOffset(flatbuffers::FlatBufferBuilder &fbb,
                        const flatbuffers::Offset<NetworkedPhysics::ServerSnapshotMessage> &snapshotOffset,
                        int messageId) {

        if (!snapshotOffset.IsNull()) {
            NetworkedPhysics::MessageBuilder messageBuilder(fbb);
            messageBuilder.add_id(messageId);
            messageBuilder.add_data_type(NetworkedPhysics::MessageType_ServerSnapshotMessage);
            messageBuilder.add_data(snapshotOffset.Union());
            return messageBuilder.Finish();
        }

        return flatbuffers::Offset<NetworkedPhysics::Message>();
    }


}


#endif //NETWORKEDPHYSICS_FLATBUFFERUTIL_H
