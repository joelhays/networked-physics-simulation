#ifndef NETWORKEDPHYSICS_MOCKPACKETFACTORY_H
#define NETWORKEDPHYSICS_MOCKPACKETFACTORY_H

#include <netinet/in.h>
#include "../../Network/IPacketFactory.h"

class MockPacketFactory : public IPacketFactory
{
public:
    static int NumCreated;
    static int NumDestroyed;

    static void ResetMock() {
        NumCreated = 0;
        NumDestroyed = 0;
    }

    MockPacketFactory() : p(Packet(sockaddr_in(), nullptr, 0)) {
    }

    Packet *Create(sockaddr_in addr, uint8_t *data, size_t data_length) override {
        NumCreated++;
        return &p;
    }

    void Destroy(Packet *&packet) override {
        NumDestroyed++;
    }

    Packet p;
};

int MockPacketFactory::NumCreated = 0;
int MockPacketFactory::NumDestroyed = 0;

#endif //NETWORKEDPHYSICS_MOCKPACKETFACTORY_H
