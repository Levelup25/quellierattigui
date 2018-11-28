#include "RandomAI.h"
#include "engine/MoveCommands.h"
#include "engine/AttackCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

RandomAI::RandomAI(State *state, Engine *engine)
{
    this->state = state;
    this->engine = engine;
}

void RandomAI::run(Character *character)
{
    vector<Command *> commands = this->listCommands(character);
    while (state->isFighting() && commands.size() > 0 && character->getPvCurrent() > 0)
    {
        engine->addCommand(commands[rand() % commands.size()]);
        while (engine->getSize() != 0);
        commands = this->listCommands(character);
    }
}
