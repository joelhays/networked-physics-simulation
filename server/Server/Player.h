#ifndef NETWORKEDPHYSICS_PLAYER_H
#define NETWORKEDPHYSICS_PLAYER_H

#include <netinet/in.h>
#include "../Simulation/Simulation.h"

struct Player {

    int id;
    int objectId;
    sockaddr_in address;

    int baselineMessageId;

    Player() {
        id = getPlayerId();
        baselineMessageId = -1;
    }

    bool operator==(const Player &rhs) {
        return id == rhs.id;
    }

private:
    static int getPlayerId() {
        static int id = 0;
        return id++;
    }
};

#endif //NETWORKEDPHYSICS_PLAYER_H
