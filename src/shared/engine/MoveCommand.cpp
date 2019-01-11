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
  type = "MoveCommand";
  this->state = state;
  this->character = character;
  this->i = i;
  this->j = j;
  this->pm = pm;
  this->reverse = reverse;
}

void MoveCommand::execute() {
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
  if (state->isFighting()) {
    if (reverse)
      character->removePm(-pm);
    else
      character->removePm(pm);

    if (state->getFight()->getTurn() == 0) {
      if (state->getTeam(character) == state->getFight()->getTeam(0))
        character->setDirection(3);
      else
        character->setDirection(0);
    }
  }

  state->moveCharacter(character, i, j);
}

void const MoveCommand::serialize(Json::Value& out) {
  // return;
  out["command"] = "MoveCommand";
  vector<Character*> characters = state->initialCharacters;
  int k = characters.size();
  while (k--)
    if (character == characters[k]) {
      out["character"] = k;
      break;
    }
  out["i"] = i;
  out["j"] = j;
  out["pm"] = pm;
}

MoveCommand* MoveCommand::deserialize(const Json::Value& in,
                                      State* state,
                                      Engine* engine) {
  Character* character =
      state->initialCharacters[in.get("character", 0).asInt()];
  float i = in.get("i", 0).asFloat();
  float j = in.get("j", 0).asFloat();
  int pm = in.get("pm", 0).asInt();
  return new MoveCommand(state, character, i, j, pm);
}