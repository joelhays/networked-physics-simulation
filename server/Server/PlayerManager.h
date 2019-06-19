#ifndef NETWORKEDPHYSICS_PLAYERMANAGER_H
#define NETWORKEDPHYSICS_PLAYERMANAGER_H


#include <netinet/in.h>
#include "Player.h"

class PlayerManager {
public:

    void Update();

    void PlayerConnected(const Player& player);
    void PlayerDisconnected(const sockaddr_in addr);

    // Finds a connected player based on socket address. Returns nullptr if no connected player found.
    Player* GetPlayerBySocketAddress(sockaddr_in addr);

    const std::vector<Player>& GetPlayers() const;

    unsigned long GetNumActivePlayers() const;
private:
    std::vector<Player> m_players;
};


#endif //NETWORKEDPHYSICS_PLAYERMANAGER_H
