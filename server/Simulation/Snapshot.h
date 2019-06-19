#ifndef NETWORKEDPHYSICS_SNAPSHOT_H
#define NETWORKEDPHYSICS_SNAPSHOT_H

#include "Simulation.h"

struct SimulationSnapshot {
    std::vector<Simulation::SimulationObject> objects;
};

#endif //NETWORKEDPHYSICS_SNAPSHOT_H
