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
      if (cmd) {
        cmd->setReverse();
        cmd->execute();
        delete cmd;
      } else
        reverse = reverse_tmp;
    } else {
      reverse_tmp = false;
      reverse = reverse_tmp;
    }
  }
  if (!reverse) {
    if (commands.size() > 0) {
      commands.front()->execute();
      rollback.push(commands.front());
      // delete commands.front();
      commands.pop();
    } else
      reverse = reverse_tmp;
  }
}

Command* Engine::getCommand() {
  if (!getSize())
    return nullptr;
  if (!reverse)
    return commands.front();
  else
    return rollback.top();
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

bool Engine::getReverse() {
  return reverse;
}

void Engine::toggleReverse() {
  reverse_tmp = !reverse_tmp;
}