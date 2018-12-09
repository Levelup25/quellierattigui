#include "MoveCommand.h"
#include <iostream>
#include "math.h"

using namespace std;
using namespace state;
using namespace engine;

MoveCommand::MoveCommand(State* state,
                         Character* character,
                         float i,
                         float j,
                         int pm,
                         bool reverse) {
  this->state = state;
  this->character = character;
  this->i = i;
  this->j = j;
  this->pm = pm;
  this->reverse = reverse;
}

void MoveCommand::execute() {
  if (state->isFighting()) {
    if (reverse)
      character->removePm(-pm);
    else
      character->removePm(pm);
  }
  float i0 = character->getI(), j0 = character->getJ();
  if (!reverse) {
    if (j > j0)
      character->setDirection(0);
    else if (i < i0)
      character->setDirection(1);
    else if (i > i0)
      character->setDirection(2);
    else if (j < j0)
      character->setDirection(3);
  } else {
    if (j > j0)
      character->setDirection(3);
    else if (i < i0)
      character->setDirection(2);
    else if (i > i0)
      character->setDirection(1);
    else if (j < j0)
      character->setDirection(0);
  }

  state->moveCharacter(character, i, j);
}