#ifndef NETWORKEDPHYSICS_MESSAGEPROCESSOR_H
#define NETWORKEDPHYSICS_MESSAGEPROCESSOR_H


#include <netinet/in.h>
#include <map>
#include "../Network/Packet.h"
#include "../Flatbuffer/message_generated.h"

struct Message {
    sockaddr_in address;
    NetworkedPhysics::MessageType messageType;
    const NetworkedPhysics::Message *message;
};

class MessageProcessor {
public:
//    void Subscribe(NetworkedPhysics::MessageType, void(*)(const Message&));
    void Subscribe(NetworkedPhysics::MessageType, std::function<void(const Message&)>);

    void ProcessPacket(Packet *packet);

private:
    void dispatch(Message message);

//    std::map<NetworkedPhysics::MessageType, std::vector<void(*)(const Message&)>> m_subscribers;
    std::map<NetworkedPhysics::MessageType, std::vector<std::function<void(const Message&)>>> m_subscribers;
};


#endif //NETWORKEDPHYSICS_MESSAGEPROCESSOR_H
