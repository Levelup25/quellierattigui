#include "FightCommand.h"

using namespace std;
using namespace state;
using namespace engine;

FightCommand::FightCommand(State* state, Team* att, Team* def) {
    this->state = state;
    this->att = att;
    this->def = def;
}

void FightCommand::execute() {
    state->setFight(new Fight(att, def));
    state->deploy();
}