#include "PlayerManager.h"

void PlayerManager::Update() {
    for (auto player : m_players) {

    }
}

void PlayerManager::PlayerConnected(const Player& player) {
    auto existingPlayer = this->GetPlayerBySocketAddress(player.address);
    if (!existingPlayer) {
        m_players.push_back(player);
    }
}

void PlayerManager::PlayerDisconnected(const sockaddr_in addr) {
    auto player = this->GetPlayerBySocketAddress(addr);
    if (player) {
        m_players.erase(remove(m_players.begin(), m_players.end(), *player), m_players.end());
    }
}

Player *PlayerManager::GetPlayerBySocketAddress(sockaddr_in addr) {
    for (int i = 0; i < m_players.size(); i++) {
        Player *player = &m_players[i];
        if (player->address.sin_port == addr.sin_port &&
            player->address.sin_addr.s_addr == addr.sin_addr.s_addr) {
            return player;
        }
    }
    return nullptr;
}

const std::vector<Player> &PlayerManager::GetPlayers() const {
    return m_players;
}

unsigned long PlayerManager::GetNumActivePlayers() const {
    return m_players.size();
}

