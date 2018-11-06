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
    state->isFighting = true;
    state->getWorld()->setFight(att, def);

}