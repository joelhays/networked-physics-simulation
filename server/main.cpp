#include "Server/Server.h"
#include "MainConfig.h"
#include "Server/MessageProcessor.h"
#include <unistd.h>

MessageProcessor SetUpMessageProcessor(Simulation &simulation, PlayerManager &playerManager, CircularArray<SimulationSnapshot> &snapshotHistory);

int main(int argc, char **argv) {

    UdpClient socket;

    PacketFactory packetFactory;

    NetworkManagerConfig networkManagerConfig{};
    networkManagerConfig.maxPacketQueueSize = MAX_PACKET_QUEUE_SIZE;
    networkManagerConfig.maxPacketSize = MAX_UDP_PACKET_SIZE;
    networkManagerConfig.serverPort = SERVER_PORT;
    networkManagerConfig.serverIpAddress = std::string{SERVER_IP_ADDRESS};

    NetworkManager networkManager(socket, packetFactory, networkManagerConfig);

    Simulation simulation;

    PlayerManager playerManager;

    CircularArray<SimulationSnapshot> snapshotHistory(480);

//    MessageProcessor messageProcessor = SetUpMessageProcessor(simulation, playerManager, snapshotHistory);

    Server server(packetFactory, networkManager, simulation, playerManager);

    printf("Server listening on %s:%d", SERVER_IP_ADDRESS, SERVER_PORT);
    fflush(stdout);

    while (!Quit) {
        server.Update();
        usleep(static_cast<unsigned int>(1000.0f * (1000.0f / TICK_RATE)));
    }

    return 0;
}

MessageProcessor SetUpMessageProcessor(Simulation &simulation, PlayerManager &playerManager, CircularArray<SimulationSnapshot> &snapshotHistory) {
    MessageProcessor messageProcessor;
    messageProcessor.Subscribe(
            NetworkedPhysics::MessageType_ClientConnectMessage,
            [&](const Message& message) {
                // create a player
                // create a simulation object
                // link the player and simulation

                Player newPlayer;
                newPlayer.objectId = simulation.GetNewPlayerObjectId();
                newPlayer.address = message.address;
                playerManager.PlayerConnected(newPlayer);
            });
    messageProcessor.Subscribe(
            NetworkedPhysics::MessageType_ClientDisconnectMessage,
            [&](const Message& message) {
                playerManager.PlayerDisconnected(message.address);
            });
    messageProcessor.Subscribe(
            NetworkedPhysics::MessageType_MessageAck,
            [&](const Message& message) {
                auto player = playerManager.GetPlayerBySocketAddress(message.address);
                if (player) {
                    auto ack = reinterpret_cast<const NetworkedPhysics::MessageAck *>(message.message->data());
                    // get simulation state based on the message id --- ack->messageId();
                    // set baseline state on player to new ack
                    if (player->baselineMessageId < ack->messageId()) {
                        player->baselineMessageId = ack->messageId();
                    }
                }
            });
    messageProcessor.Subscribe(
            NetworkedPhysics::MessageType_ClientInputMessage,
            [&](const Message& message) {
                auto input = reinterpret_cast<const NetworkedPhysics::ClientInputMessage *>(message.message->data());
                auto player = playerManager.GetPlayerBySocketAddress(message.address);
                if (player) {
                    // update the simulation
                    simulation.ApplyForce(player->objectId, input);
                }
            });
    messageProcessor.Subscribe(
            NetworkedPhysics::MessageType_ResetSimulationMessage,
            [&](const Message& message) {
                snapshotHistory.clear();
                simulation.Reset();
            });

    return messageProcessor;
}