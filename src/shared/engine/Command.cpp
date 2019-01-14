#include "Command.h"
#include <string.h>
#include <iostream>
#include "AttackCommand.h"
#include "DamageCommand.h"
#include "FightCommand.h"
#include "MoveCommand.h"
#include "MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;

string Command::getType() {
  if (!this)
    return "";
  return type;
}

Character* Command::getCharacter() {
  return character;
}

void Command::setReverse(bool reverse) {
  this->reverse = reverse;
}

void Command::execute() {}

void const Command::serialize(Json::Value& out) {}

Command* Command::deserialize(const Json::Value& in,
                              State* state,
                              Engine* engine) {
  string command_str = in.get("command", 0).asString();
  Command* cmd = nullptr;
  if (command_str.compare("AttackCommand") == 0)
    cmd = AttackCommand::deserialize(in, state, engine);
  else if (command_str.compare("DamageCommand") == 0)
    cmd = DamageCommand::deserialize(in, state, engine);
  else if (command_str.compare("FightCommand") == 0)
    cmd = FightCommand::deserialize(in, state, engine);
  else if (command_str.compare("MoveCommand") == 0)
    cmd = MoveCommand::deserialize(in, state, engine);
  else if (command_str.compare("MoveCommands") == 0)
    cmd = MoveCommands::deserialize(in, state, engine);

  return cmd;
}