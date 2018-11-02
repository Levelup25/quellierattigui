#include "MoveCommand.h"
#include <math.h>

using namespace std;
using namespace state;
using namespace engine;

MoveCommand::MoveCommand(State* state, Character* character, float i, float j) {
    this->state = state;
    this->character = character;
    this->i = i;
    this->j = j;
}

void MoveCommand::execute() {
    state->getWorld()->moveCharacter(character, i, j);
}