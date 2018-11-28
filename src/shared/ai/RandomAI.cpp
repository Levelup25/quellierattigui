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
    //    int n = state->getN(), m = state->getM(), X = character->getI(), Y = character->getJ(), pm = character->getPm(), pa = character->getPa();
    //    int r1, r2;
    //    do {
    //        r1 = -pm + rand() % (2 * pm + 1);
    //        r2 = -pm + rand() % (2 * pm + 1);
    //    } while ((abs(r1) + abs(r2) > pm) || (X % n + r1 < 0) || (X % n + r1 >= n) || (Y % m + r2 < 0) || (Y % m + r2 >= m) || (state->getCell(X + r1, Y + r2)->getContent() > 0));
    //    X = character->getI() + r1, Y = character->getJ() + r2;
    //    engine->addCommand(new MoveCommands(state, engine, character, X, Y));
    //
    //    vector<Ability*> abilities = character->getWeapon()->getAbilities();
    //    vector<int> possible;
    //    Ability* a;
    //    vector<vector<int>> targets;
    //    vector<vector<int>> zones;
    //    int i;
    //    do {
    //        i = 0;
    //        a = nullptr;
    //        possible.clear();
    //        for (auto a : abilities) {
    //            if (a->getPa() <= pa) possible.push_back(i);
    //            i++;
    //        }
    //        if (possible.size() > 0) {
    //            int ind = possible[rand() % possible.size()];
    //            a = abilities[ind];
    //            pa -= a->getPa();
    //            targets = a->getTargetZone({X, Y});
    //            int r;
    //            do {
    //                r = rand() % targets.size();
    //            } while (targets[r][0] % n < 0 || targets[r][0] % n >= n || targets[r][1] % m < 0 || targets[r][1] % m >= m);
    //            zones = a->getEffectZone(targets[r]);
    //            do {
    //                r = rand() % zones.size();
    //            } while (zones[r][0] % n < 0 || zones[r][0] % n >= n || zones[r][1] % m < 0 || zones[r][1] % m >= m);
    //            engine->addCommand(new AttackCommand(state, engine, character, zones[r], ind));
    //        }
    //    } while (possible.size() > 0);
}
