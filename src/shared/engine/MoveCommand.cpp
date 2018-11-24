#include "MoveCommand.h"
#include "math.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

MoveCommand::MoveCommand(State* state, Character* character, float i, float j)
{
    this->state = state;
    this->character = character;
    this->i = i;
    this->j = j;
}

void MoveCommand::execute()
{
    float i0 = character->getI(), j0 = character->getJ();
    if (j > j0)
        character->setDirection(0);
    else if (i < i0)
        character->setDirection(1);
    else if (i > i0)
        character->setDirection(2);
    else if (j < j0)
        character->setDirection(3);

    state->moveCharacter(character, i, j);

}