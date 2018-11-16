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
    if (!state->isFighting()) {
        cout << "fight1" << endl;
        state->setFight(new Fight(att, def, 3));
        state->deploy();
        cout << "fight2" << endl;
    } else {
        cout << "fight3" << endl;
        state->getFight()->endTurn();
        state->etatCombat = 0;
        cout << "fight4" << endl;
    }
}