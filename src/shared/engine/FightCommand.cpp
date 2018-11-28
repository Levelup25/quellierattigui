#include "FightCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

FightCommand::FightCommand(State* state, Team* att, Team* def) {
  this->state = state;
  this->character = nullptr;
  this->att = att;
  this->def = def;
}

void FightCommand::execute() {
  if (!state->isFighting() && att != nullptr && def != nullptr) {
    state->setFight(new Fight(att, def, 3));
    state->deploy();
  } else if (state->isFighting() && att == nullptr && def == nullptr) {
    state->getFight()->endTurn();
    state->endFight();
    state->etatCombat = 0;
  }
}