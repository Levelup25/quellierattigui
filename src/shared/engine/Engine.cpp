#include "Engine.h"

using namespace std;
using namespace state;
using namespace engine;

void Engine::addCommand(Command* command) {
    commands.push_back(command);
}

void Engine::runCommand() {
    commands[0]->execute();
    commands.erase(commands.begin());
}

void Engine::runCommands() {
    for (auto cmd = commands.begin(); cmd != commands.end(); ++cmd) {
        (*cmd)->execute();
    }
    commands.clear();
}

size_t Engine::getSize() {
    return commands.size();
}