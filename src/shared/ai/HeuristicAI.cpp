#include "HeuristicAI.h"
#include "engine/MoveCommands.h"
#include "engine/AttackCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

HeuristicAI::HeuristicAI(State* state, Engine* engine) {
    this->state = state;
    this->engine = engine;
}

void HeuristicAI::run(Character* character) {
    vector<Command*> commands = this->listCommands(character);
    for (auto command : commands) {

    }
    //    while (commands.size() > 0 && character->getPv() > 0) {
    //        engine->addCommand(commands[rand() % commands.size()]);
    //        state->resetContents();
    //        while (engine->getSize() != 0);
    //        commands = this->listCommands(character);
    //    }
}



