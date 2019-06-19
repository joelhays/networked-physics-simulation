#include "MessageProcessor.h"
#include "../Flatbuffer/message_generated.h"

//void MessageProcessor::Subscribe(NetworkedPhysics::MessageType messageType, void (*callback)(const Message &)) {
//    if (m_subscribers.count(messageType) == 0) {
//        std::vector<void(*)(const Message&)> callbacks;
//        callbacks.push_back(callback);
//        m_subscribers.insert(std::make_pair(messageType, callbacks));
//    } else {
//        m_subscribers[messageType].push_back(callback);
//    }
//}

void MessageProcessor::ProcessPacket(Packet *packet) {
    auto message = NetworkedPhysics::GetMessage(packet->data);

    Message msg{packet->address, message->data_type(), message};
    dispatch(msg);
}

void MessageProcessor::dispatch(Message message) {
    for (auto callback : m_subscribers[message.messageType]) {
        callback(message);
    }
}

void MessageProcessor::Subscribe(NetworkedPhysics::MessageType messageType, std::function<void(const Message &)> callback) {
    if (m_subscribers.count(messageType) == 0) {
        std::vector<std::function<void(const Message &)>> callbacks;
        callbacks.push_back(callback);
        m_subscribers.insert(std::make_pair(messageType, callbacks));
    } else {
        m_subscribers[messageType].push_back(callback);
    }
}


