#ifndef NETWORKEDPHYSICS_NETWORKMANAGERCONFIG_H
#define NETWORKEDPHYSICS_NETWORKMANAGERCONFIG_H

#include <string>

struct NetworkManagerConfig {
    int maxPacketQueueSize;
    int maxPacketSize;

    int serverPort;
    std::string serverIpAddress;
};

#endif //NETWORKEDPHYSICS_NETWORKMANAGERCONFIG_H
