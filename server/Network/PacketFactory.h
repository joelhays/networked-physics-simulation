#ifndef NETWORKEDPHYSICS_PACKETFACTORY_H
#define NETWORKEDPHYSICS_PACKETFACTORY_H


#include <vector>
#include <memory>
#include <netinet/in.h>
#include "IPacketFactory.h"
#include "Packet.h"

class PacketFactory : public IPacketFactory {
public:
    ~PacketFactory() override;

    Packet *Create(sockaddr_in addr, uint8_t *data, size_t data_length) override;
    void Destroy(Packet *&packet) override;

private:
    std::vector<std::unique_ptr<Packet>> m_packets;

};


#endif //NETWORKEDPHYSICS_PACKETFACTORY_H
