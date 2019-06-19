#ifndef NETWORKEDPHYSICS_NETWORKMANAGER_H
#define NETWORKEDPHYSICS_NETWORKMANAGER_H


#include <queue>
#include "../Network/ISocketClient.h"
#include "../Network/IPacketFactory.h"
#include "../Flatbuffer/message_generated.h"
#include "NetworkManagerConfig.h"


class NetworkManager {
public:
    NetworkManager(ISocketClient &socket, IPacketFactory &packetFactory, NetworkManagerConfig &config);

    // Sends and receives all buffered packets
    void Update();

    // Returns a buffered packet from the inbound queue if one exists, otherwise returns a nullptr
    Packet *ReceivePacket();

    // Adds a packet to the outbound queue to be sent on the next Update cycle
    void SendPacket(Packet *pPacket);

    NetworkManagerConfig Config() const;

private:
    NetworkManager() = default;

    ISocketClient &m_socket;
    IPacketFactory &m_packetFactory;

    NetworkManagerConfig &m_config;

    std::queue<Packet *> m_inboundPackets;
    std::queue<Packet *> m_outboundPackets;

    void SendPackets();

    void ReceivePackets();

    std::unique_ptr<uint8_t[]> m_receiveBuffer;
};


#endif //NETWORKEDPHYSICS_NETWORKMANAGER_H
