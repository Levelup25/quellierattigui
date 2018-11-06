#include "State.h"

using namespace std;
using namespace state;

State::State(size_t i, size_t j) {
  world = new World(i, j);
}

World* State::getWorld() {
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

Fight* State::getFight() {
  return fight;
}

void State::setFight(Fight* fight) {
  this->fight = fight;
}

void State::delFight() {
  fight = nullptr;
}

bool State::isFighting() {
  return fight != nullptr;
}