#include "AnimationCommand.h"
#include <algorithm>

using namespace std;
using namespace state;
using namespace engine;

AnimationCommand::AnimationCommand(State* state,
                                   vector<vector<int>> positions,
                                   vector<int> directions,
                                   int element,
                                   int lv,
                                   int dmg,
                                   bool reverse) {
  this->state = state;
  this->character = nullptr;
  this->positions = positions;
  this->directions = directions;
  this->element = element;
  this->lv = lv;
  this->reverse = reverse;
  this->dmg = reverse ? -dmg : dmg;
}

void AnimationCommand::execute() {
  state->animations.clear();
  int i = 0;
  for (auto pos : positions) {
    state->animations.push_back({pos[0], pos[1], directions[i++], element, lv});
    if (dmg != 0) {
      Character* c = state->getCharacter(pos[0], pos[1]);
      vector<Character*> fighters;
      if (!reverse)
        fighters = state->getFight()->getFightingCharacters();
      else
        fighters = state->getFight()->getCharacters();
      if (find(fighters.begin(), fighters.end(), c) != fighters.end()) {
        c->removePv(dmg);
        if (c->getPvCurrent() <= 0)
          state->getCell(c->getI(), c->getJ())->setContent(nothing);
      }
    }
  }
  if (state->isFighting())
    state->endFight();
}
