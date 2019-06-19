#ifndef NETWORKEDPHYSICS_IPACKETFACTORY_H
#define NETWORKEDPHYSICS_IPACKETFACTORY_H

#include "Packet.h"

struct IPacketFactory {
public:
    virtual ~IPacketFactory() = default;

    // Constructs a packet that can be used to send and receive data on a socket
    // A list of active packets are kept internally
    virtual Packet *Create(sockaddr_in addr, uint8_t *data, size_t data_length) = 0;

    // Destroys a packet
    // This should be called for every packet once it's done being used, but if a packet is not destroyed,
    // then it will be destroyed when this class is destroyed
    virtual void Destroy(Packet *&packet) = 0;
};


#endif //NETWORKEDPHYSICS_IPACKETFACTORY_H
