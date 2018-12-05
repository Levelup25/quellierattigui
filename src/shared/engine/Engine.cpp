#include "Engine.h"

using namespace std;
using namespace state;
using namespace engine;

void Engine::addCommand(Command* command, bool reverse) {
  if (!reverse)
    commands.push_back(command);
  else
    rollback.push_back(command);
}

void Engine::runCommand(bool reverse) {
  if (!reverse) {
    if (commands.size() > 0) {
      commands[0]->execute();
      delete commands[0];
      commands.erase(commands.begin());
    }
  } else {
    if (rollback.size() > 0) {
      rollback[rollback.size() - 1]->execute();
      delete rollback[rollback.size() - 1];
      rollback.pop_back();
    }
  }
}

void Engine::clearCommands(bool reverse) {
  if (!reverse)
    commands.clear();
  else
    rollback.clear();
}

size_t Engine::getSize(bool reverse) {
  if (!reverse)
    return commands.size();
  else
    return rollback.size();
}