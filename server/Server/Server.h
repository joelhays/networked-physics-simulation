#ifndef NETWORKEDPHYSICS_SERVER_H
#define NETWORKEDPHYSICS_SERVER_H

#include "NetworkManager.h"
#include "Player.h"
#include "../Simulation/Simulation.h"
#include "../Simulation/Snapshot.h"
#include "../Util/CircularArray.h"
#include "../Network/UdpClient.h"
#include "../Network/PacketFactory.h"
#include "PlayerManager.h"

class Server {
public:
    Server(IPacketFactory &, NetworkManager &, Simulation &, PlayerManager &);

    ~Server();

    // Processes network packets, updates the simulation, and send snapshot deltas to connected clients.
    void Update();

private:
    Server() = default;

    IPacketFactory &m_packetFactory;
    NetworkManager &m_networkManager;

    Simulation &m_simulation;

    PlayerManager &m_playerManager;

    CircularArray<SimulationSnapshot> m_snapshotHistory;


private:
    // Sends interpolated snapshots to connected players based on that player's most recent ack.
    void SendInterpolatedSnapshots();
};


#endif //NETWORKEDPHYSICS_SERVER_H
