#include "Server.h"
#include "../Util/FlatbufferUtil.h"

Server::Server(IPacketFactory &packetFactory, NetworkManager &networkManager, Simulation &simulation, PlayerManager &playerManager)
        : m_snapshotHistory(480),
          m_packetFactory(packetFactory),
          m_networkManager(networkManager),
          m_simulation(simulation),
          m_playerManager(playerManager) {
}

Server::~Server() {
}

void Server::Update() {

    // send and receive all packets
    m_networkManager.Update();

    //iterate through all received packets
    while (true) {
        auto packet = m_networkManager.ReceivePacket();
        if (!packet)
            break;

        auto message = NetworkedPhysics::GetMessage(packet->data);
        switch (message->data_type()) {
            case NetworkedPhysics::MessageType_ClientConnectMessage: {
                // create a new player proxy
                Player newPlayer;
                newPlayer.objectId = m_simulation.GetNewPlayerObjectId();
                newPlayer.address = packet->address;
                m_playerManager.PlayerConnected(newPlayer);
                break;
            }
            case NetworkedPhysics::MessageType_ClientDisconnectMessage: {
                m_playerManager.PlayerDisconnected(packet->address);
                break;
            }
            case NetworkedPhysics::MessageType_MessageAck: {
                auto player = m_playerManager.GetPlayerBySocketAddress(packet->address);
                if (player) {
                    auto ack = reinterpret_cast<const NetworkedPhysics::MessageAck *>(message->data());
                    // get simulation state based on the message id --- ack->messageId();
                    // set baseline state on player to new ack
                    if (player->baselineMessageId < ack->messageId()) {
                        player->baselineMessageId = ack->messageId();
                    }
                }

                break;
            }
            case NetworkedPhysics::MessageType_ClientInputMessage: {
//                printf("Got Input message.\n");

                auto input = reinterpret_cast<const NetworkedPhysics::ClientInputMessage *>(message->data());
                Player *player = m_playerManager.GetPlayerBySocketAddress(packet->address);
                if (player) {
                    // update the simulation
                    m_simulation.ApplyForce(player->objectId, input);
                }

                break;
            }
            case NetworkedPhysics::MessageType_ResetSimulationMessage: {
                m_snapshotHistory.clear();
                m_simulation.Reset();
                break;
            }
            default : {
                printf("received unhandled message type");
            }
        }

        m_packetFactory.Destroy(packet);
    }

    // now send all updated packets from the simulation
    if (m_playerManager.GetNumActivePlayers() > 0) {
        m_simulation.Update();

        SendInterpolatedSnapshots();
    }
}

void Server::SendInterpolatedSnapshots() {
    auto simObjects = m_simulation.GetSimulationObjects();

    bool packetSent = false;

    flatbuffers::FlatBufferBuilder builders[256];

    for (auto player : m_playerManager.GetPlayers()) {


        flatbuffers::FlatBufferBuilder fbb;
        flatbuffers::Offset<NetworkedPhysics::Entity> entitiesVector[simObjects.size()];
        auto snapshot = m_snapshotHistory.getValue(player.baselineMessageId);

        for (int i = 0, sent = 0, created = 0; i < simObjects.size(); i++) {
            auto simObject = simObjects[i];

            Simulation::SimulationObject baseline;
            if (snapshot) {
                baseline = snapshot->objects[i];
            }

            flatbuffers::Offset<NetworkedPhysics::Vec3> positionOffset =
                    FlatbufferUtil::CreatePositionOffset(fbb, baseline, simObject);

            flatbuffers::Offset<NetworkedPhysics::Vec4> rotationOffset =
                    FlatbufferUtil::CreateRotationOffset(fbb, baseline, simObject);

            flatbuffers::Offset<NetworkedPhysics::Vec3> scaleOffset =
                    FlatbufferUtil::CreateScaleOffset(fbb, baseline, simObject);

            flatbuffers::Offset<NetworkedPhysics::Transform> transformOffset =
                    FlatbufferUtil::CreateTransformOffset(fbb, positionOffset, rotationOffset, scaleOffset);

            flatbuffers::Offset<NetworkedPhysics::Entity> entityOffset =
                    FlatbufferUtil::CreateEntityOffset(fbb, transformOffset, simObject, player);

            if (!entityOffset.IsNull()) {
                entitiesVector[created] = entityOffset;
                created++;
            }

            if (fbb.GetSize() >= m_networkManager.Config().maxPacketSize || i == simObjects.size() - 1) {
                flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<NetworkedPhysics::Entity>>> entitiesVectorOffset;
                if (created > 0)
                    entitiesVectorOffset = fbb.CreateVector(entitiesVector + sent, static_cast<size_t>(created - sent));

                flatbuffers::Offset<NetworkedPhysics::ServerSnapshotMessage> snapshotOffset =
                        FlatbufferUtil::CreateSnapshotOffset(fbb, entitiesVectorOffset);

                flatbuffers::Offset<NetworkedPhysics::Message> messageOffset =
                        FlatbufferUtil::CreateMessageOffset(fbb, snapshotOffset, m_snapshotHistory.getSequence());

                if (!messageOffset.IsNull()) {
                    fbb.Finish(messageOffset);

                    auto packet = m_packetFactory.Create(player.address, fbb.GetBufferPointer(), fbb.GetSize());
                    m_networkManager.SendPacket(packet);

                    packetSent = true;
                }

                sent = created;

                fbb.Reset();
            }
        }
    }

    if (packetSent) {
        SimulationSnapshot newSnapshot;
        newSnapshot.objects = simObjects;
        m_snapshotHistory.insert(newSnapshot);
    }
}

