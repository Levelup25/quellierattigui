#include "AI.h"
#include <iostream>
#include "engine/AttackCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

vector<Command*> AI::listCommands(Character* character, int type) {
  vector<Command*> commands{};
  int X = character->getI(), Y = character->getJ(), n = state->getN(),
      m = state->getM(), pa = character->getPaCurrent(),
      pm = character->getPmCurrent();
  if (type != 1) {
    if (pm > 0) {
      for (int a = -pm; a <= pm; a++) {
        for (int b = abs(a) - pm; b <= pm - abs(a); b++) {
          if ((a != 0 || b != 0) && (X + a) / n == X / n && X + a >= 0 &&
              (Y + b) / m == Y / m && Y + b >= 0) {
            MoveCommands* mvcmds =
                new MoveCommands(state, engine, character, a, b);
            if ((int)mvcmds->getPath().size() <= pm &&
                state->getCell(X + a, Y + b)->getContent() == 0)
              commands.push_back(mvcmds);
          }
        }
      }
    }
  }
  if (type != 0) {
    vector<Ability*> abilities = character->getWeapon()->getAbilities();
    vector<vector<int>> targets;
    for (unsigned int i = 0; i < abilities.size(); i++) {
      if (pa >= (int)abilities[i]->getPa()) {
        targets = abilities[i]->getTargetZone({X, Y});
        for (auto target = targets.end() - 1; target >= targets.begin();
             target--) {
          if ((*target)[0] / n != X / n || (*target)[0] < 0 ||
              (*target)[1] / m != Y / m || (*target)[1] < 0)
            targets.erase(target);
        }
        for (auto target : targets) {
          target[0] -= X, target[1] -= Y;
          commands.push_back(
              new AttackCommand(state, engine, character, target, i));
        }
      }
    }
  }
  return commands;
}

vector<Character*> AI::getTurnOrder(vector<Character*> characters) {
  return characters;
}

void AI::run(Character* character) {}