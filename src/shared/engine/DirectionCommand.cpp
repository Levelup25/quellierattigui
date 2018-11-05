#include "DirectionCommand.h"

using namespace std;
using namespace state;
using namespace engine;

DirectionCommand::DirectionCommand(Character* character, int direction) {
    this->character = character;
    this->direction = direction;
}

void DirectionCommand::execute() {
    character->setDirection(direction);
}
