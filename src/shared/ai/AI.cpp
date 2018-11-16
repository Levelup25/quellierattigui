#include "AI.h"
#include "engine/MoveCommands.h"
#include "engine/AttackCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

vector<Command*> AI::listCommands(Character* character) {
    vector<Command*> commands{};
    int X = character->getI(), Y = character->getJ(), n = state->getN(), m = state->getM(), pa = character->getPa();
    if (character->getPm() > 0) {
        if ((X + 1) % n != 0) if (state->getCell(X + 1, Y)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X + 1, Y));
        if (X % n != 0) if (state->getCell(X - 1, Y)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X - 1, Y));
        if ((Y + 1) % m != 0) if (state->getCell(X, Y + 1)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X, Y + 1));
        if (Y % m != 0) if (state->getCell(X, Y - 1)->getContent() == nothing)commands.push_back(new MoveCommands(state, engine, character, X, Y - 1));
    }
    vector<Ability*> abilities = character->getWeapon()->getAbilities();
    vector<vector<int>> targets;
    for (int i = 0; i < abilities.size(); i++) {
        if (pa >= abilities[i]->getPa()) {
            targets = abilities[i]->getTargetZone({X, Y});
            for (auto target = targets.end() - 1; target >= targets.begin(); target--) {
                if ((*target)[0] / n != X / n || (*target)[1] / m != Y / m) targets.erase(target);
            }
            for (auto target : targets) commands.push_back(new AttackCommand(state, engine, character, target, i));
        }
    }
    return commands;
}

void AI::run(Character* character) {

}