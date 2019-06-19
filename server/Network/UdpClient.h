#ifndef NETWORKEDPHYSICS_UDPCLIENT_H
#define NETWORKEDPHYSICS_UDPCLIENT_H

#include "ISocketClient.h"

class UdpClient : public ISocketClient {

public:
    UdpClient();
    ~UdpClient() override;

    void Connect(const char *ipAddress, const u_short port) override;
    int Send(const sockaddr_in &to, const uint8_t *buffer, const size_t bufferSize) const override;
    int Receive(sockaddr_in &from, uint8_t *buffer, const size_t bufferSize) const override;

private:
    in_addr_t m_ipAddress;
    u_short m_port;

    int m_socket;

    // Creates and binds the socket and sets it to non-blocking
    void initializeSocket();
    void cleanupSocket();
};


#endif //NETWORKEDPHYSICS_UDPCLIENT_H
