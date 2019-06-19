#ifndef NETWORKEDPHYSICS_GLOBAL_H
#define NETWORKEDPHYSICS_GLOBAL_H

#include "Network/PacketFactory.h"

static const float TICK_RATE = 60;
static char *SERVER_IP_ADDRESS = const_cast<char *>("0.0.0.0");
static const int SERVER_PORT = 34001;
static const int MAX_UDP_PACKET_SIZE = 1200;
static const int MAX_PACKET_QUEUE_SIZE = 256;

static bool Quit = false;

#endif //NETWORKEDPHYSICS_GLOBAL_H