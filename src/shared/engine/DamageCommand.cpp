#include "DamageCommand.h"
#include <algorithm>
#include "FightCommand.h"

using namespace std;
using namespace state;
using namespace engine;

DamageCommand::DamageCommand(State* state,
                             Engine* engine,
                             vector<vector<int>> positions,
                             vector<int> directions,
                             int element,
                             int lv,
                             int dmg,
                             bool reverse) {
  this->state = state;
  this->engine = engine;
  this->character = nullptr;
  this->positions = positions;
  this->directions = directions;
  this->element = element;
  this->lv = lv;
  this->reverse = reverse;
  this->dmg = dmg;
}

void DamageCommand::execute() {
  shared_ptr<Fight> fight = state->getFight();
  state->animations.clear();
  vector<Character*> fighters;
  if (reverse)
    dmg = -dmg;
  int i = 0;
  for (auto pos : positions) {
    if (directions[0] != -1)
      state->animations.push_back(
          {pos[0], pos[1], directions[i++], element, lv});
    if (dmg != 0) {
      Character* c = state->getCharacter(pos[0], pos[1]);
      if (!reverse)
        fighters = fight->getFightingCharacters();
      else {
        fighters = fight->getCharacters();
      }
      if (find(fighters.begin(), fighters.end(), c) != fighters.end()) {
        c->removePv(dmg);
        if (c->getPvCurrent() <= 0)
          state->getCell(c->getI(), c->getJ())->setContent(nothing);
        else
          state->getCell(c->getI(), c->getJ())->setContent(perso);
      }
    }
  }

  if (!dmg && fight &&
      (fight->getFightingCharacters(0).size() == 0 ||
       fight->getFightingCharacters(1).size() == 0)) {
    state->endFight();
    engine->clearCommands(true);
  }
}
