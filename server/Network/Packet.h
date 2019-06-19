#ifndef NETWORKEDPHYSICS_PACKET_H
#define NETWORKEDPHYSICS_PACKET_H

#include <cstdint>
#include <cstring>

struct Packet {
    Packet(sockaddr_in address, uint8_t *data, int data_length) {
        static long id = 0;
        this->id = id++;

        this->address = address;
        this->data_length = data_length;
        this->data = new uint8_t[data_length];
        memcpy(this->data, data, (size_t)data_length);
    }

    ~Packet() {
        delete[] this->data;
    }

    long id;
    sockaddr_in address;
    uint8_t *data;
    int data_length;
};

#endif //NETWORKEDPHYSICS_PACKET_H
