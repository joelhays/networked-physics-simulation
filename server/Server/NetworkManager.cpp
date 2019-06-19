#include <netinet/in.h>
#include "NetworkManager.h"

NetworkManager::NetworkManager(ISocketClient &socket, IPacketFactory &packetFactory, NetworkManagerConfig &config)
        : m_socket(socket),
          m_packetFactory(packetFactory),
          m_config(config) {
    m_socket.Connect(m_config.serverIpAddress.c_str(), static_cast<const unsigned short>(m_config.serverPort));
    m_receiveBuffer = std::make_unique<uint8_t[]>(static_cast<size_t>(m_config.maxPacketSize));
}

void NetworkManager::Update() {
    SendPackets();
    ReceivePackets();
}

void NetworkManager::SendPackets() {
    while (m_outboundPackets.size() > 0) {
        auto packet = m_outboundPackets.front();
        m_outboundPackets.pop();

        m_socket.Send(packet->address, packet->data, static_cast<size_t>(packet->data_length));

        m_packetFactory.Destroy(packet);
    }
}

void NetworkManager::ReceivePackets() {
    int numPacketsReceived = 0;

    while (numPacketsReceived < m_config.maxPacketQueueSize) {

        sockaddr_in from{};
        int bytes_received = m_socket.Receive(from, m_receiveBuffer.get(), static_cast<const size_t>(m_config.maxPacketSize));
        if (bytes_received == 0) {
            break;
        } else {
            auto packet = m_packetFactory.Create(from, m_receiveBuffer.get(), static_cast<size_t>(bytes_received));
            m_inboundPackets.push(packet);
        }

    }
}

Packet *NetworkManager::ReceivePacket() {
    if (m_inboundPackets.size() == 0)
        return nullptr;

    auto packet = m_inboundPackets.front();
    m_inboundPackets.pop();
    return packet;
}

void NetworkManager::SendPacket(Packet *pPacket) {
    m_outboundPackets.push(pPacket);
}

NetworkManagerConfig NetworkManager::Config() const {
    return m_config;
}


