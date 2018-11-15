#include "AI.h"
#include "engine/MoveCommands.h"
#include "engine/AttackCommand.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

vector<Command*> AI::listCommands(Character* character) {
    vector<Command*> commands;
    int X = character->getI(), Y = character->getJ(), pa = character->getPa();
    if (character->getPm() > 0) {
        if (X < state->getI() - 1 && state->getCell(X + 1, Y)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X + 1, Y));
        if (X > 0 && state->getCell(X - 1, Y)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X - 1, Y));
        if (Y < state->getJ() - 1 && state->getCell(X, Y + 1)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X, Y + 1));
        if (Y > 0 && state->getCell(X, Y - 1)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X, Y - 1));
    }
    vector<Ability*> abilities = character->getWeapon()->getAbilities();
    vector<vector<int>> targets;
    for (int i = 0; i < abilities.size(); i++) {
        if (pa >= abilities[i]->getPa()) {
            targets = abilities[i]->getTargetZone({X, Y});
            for (auto target = targets.end() - 1; target >= targets.begin(); target--) {
                if ((*target)[0] / state->getN() != X / state->getN() || (*target)[1] / state->getM() != Y / state->getM()) targets.erase(target);
            }
            for (auto target : targets) commands.push_back(new AttackCommand(state, engine, character, target, i));
        }
    }
    return commands;
}

void AI::run(Character* character) {

}