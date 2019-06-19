#include <algorithm>
#include "PacketFactory.h"

Packet *PacketFactory::Create(sockaddr_in address, uint8_t *data, size_t data_length) {
    std::unique_ptr<Packet> packet = std::make_unique<Packet>(address, data, data_length);
    m_packets.push_back(std::move(packet));
    return m_packets.back().get();
}

void PacketFactory::Destroy(Packet *&packet) {
    std::vector<std::unique_ptr<Packet>>::iterator object =
            std::find_if(m_packets.begin(), m_packets.end(),
                         [&](const std::unique_ptr<Packet> &obj) { return obj->id == packet->id; }
            );
    m_packets.erase(std::remove(m_packets.begin(), m_packets.end(), *object));

    packet = nullptr;
}

PacketFactory::~PacketFactory() {
    for (std::unique_ptr<Packet> &packet : m_packets) {
        delete[] packet->data;
    }
    m_packets.clear();
}
