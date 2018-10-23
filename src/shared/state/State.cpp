#include "State.h"

using namespace std;
using namespace state;

State::State() {
    World *w = new World(12, 12);
    world = w;
}

void State::setFight(bool b) {
    isFighting = b;
}

void State::setInventory(bool b) {
    isInventoryOpened = b;
}

World *State::getWorld() {
    return world;
}

int State::getEpoch() const {
    return epoch;
}

void State::setEpoch(int epoch) {
    this->epoch = epoch;
}

int State::getEpochRate() const {
    return epochRate;
}

void State::setEpochRate(int epochRate) {
    this->epochRate = epochRate;
}