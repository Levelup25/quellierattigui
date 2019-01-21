#include "RandomAI.h"
#include <algorithm>
#include <iostream>
#include "AI.h"
#include "engine/AttackCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

RandomAI::RandomAI(State* state, Engine* engine) {
  this->state = state;
  this->engine = engine;
}

vector<Character*> RandomAI::getTurnOrder(vector<Character*> characters) {
  vector<Character*> v = characters;
  random_shuffle(v.begin(), v.end());
  return v;
}

bool RandomAI::run(Character* character) {
  // vector<Command*> commands = this->listCommands(character);
  // while (state->isFighting() && commands.size() > 0 &&
  //        character->getPvCurrent() > 0) {
  //   engine->addCommand(commands[rand() % commands.size()]);
  //   while (engine->getSize() != 0)
  //     ;
  //   commands = this->listCommands(character);
  // }

  if (!(state->isFighting() && character->getPvCurrent() > 0))
    return false;
  vector<Command*> commands = this->listCommands(character);
  if (!commands.size())
    return false;
  engine->addCommand(commands[rand() % commands.size()]);
  return true;
}
