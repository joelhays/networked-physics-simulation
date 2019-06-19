#ifndef NETWORKEDPHYSICS_MOCKSOCKETCLIENT_H
#define NETWORKEDPHYSICS_MOCKSOCKETCLIENT_H

#include "../../Network/ISocketClient.h"

class MockSocketClient : public ISocketClient
{
public:
    static int NumSent;
    static int NumReceived;

    static void ResetMock() {
        NumSent = 0;
        NumReceived = 0;
    }

    void Connect(const char *ipAddress,  const unsigned short port) override {

    }
    int Send(const sockaddr_in &to, const uint8_t *buffer, const size_t bufferSize) const override {
        NumSent++;
        return 1;
    }
    int Receive(sockaddr_in &from, uint8_t *buffer, const size_t bufferSize) const override {
        if (NumReceived <= 1) {
            NumReceived++;
            return 1;
        }
        return 0;
    }
};


int MockSocketClient::NumSent = 0;
int MockSocketClient::NumReceived = 0;

#endif //NETWORKEDPHYSICS_MOCKSOCKETCLIENT_H
