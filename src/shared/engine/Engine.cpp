#include "Engine.h"

using namespace std;
using namespace state;
using namespace engine;

void Engine::addCommand(Command* command) {
  if (!reverse)
    commands.push(command);
  else
    rollback.push(command);
}

void Engine::runCommand() {
  if (reverse) {
    if (rollback.size() > 0) {
      Command* cmd = rollback.top();
      rollback.pop();
      cmd->setReverse();
      cmd->execute();
      delete cmd;
    } else
      reverse = !reverse;
  }
  if (!reverse) {
    if (commands.size() > 0) {
      commands.front()->execute();
      rollback.push(commands.front());
      // delete commands.front();
      commands.pop();
    }
  }
}

void Engine::clearCommands(bool b) {
  if (!b)
    while (commands.size())
      commands.pop();
  else
    while (rollback.size())
      rollback.pop();
}

size_t Engine::getSize() {
  if (!reverse)
    return commands.size();
  else
    return rollback.size();
}