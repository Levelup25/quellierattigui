#include "FightCommand.h"

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
    if (!state->isFighting()) {
        state->setFight(new Fight(att, def, 3));
        state->deploy();
    } else {
        state->getFight()->endTurn();
        state->etatCombat = 0;
    }
}