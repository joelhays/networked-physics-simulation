#ifndef NETWORKEDPHYSICS_ISOCKETCLIENT_H
#define NETWORKEDPHYSICS_ISOCKETCLIENT_H

#include <netinet/in.h>
#include "Packet.h"

struct ISocketClient
{
    virtual ~ISocketClient() = default;

    // Connects to the specified socket
    virtual void Connect(const char *ipAddress,  const unsigned short port) = 0;

    // Sends a packet to the address specified in the packet information
    virtual int Send(const sockaddr_in &to, const uint8_t *buffer, const size_t bufferSize) const = 0;

    // Checks for any data on the socket adn returns an easily consumable representation of the packet,
    // otherwise returns a nullptr if no data available
    virtual int Receive(sockaddr_in &from, uint8_t *buffer, const size_t bufferSize) const = 0;
};

#endif //NETWORKEDPHYSICS_ISOCKETCLIENT_H
