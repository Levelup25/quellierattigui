#include "AnimationCommand.h"

using namespace std;
using namespace state;
using namespace engine;

AnimationCommand::AnimationCommand(State* state,
                                   vector<vector<int>> positions,
                                   vector<int> directions,
                                   int element,
                                   int lv) {
  this->state = state;
  this->character = nullptr;
  this->positions = positions;
  this->directions = directions;
  this->element = element;
  this->lv = lv;
}

void AnimationCommand::execute() {
  state->animations.clear();
  int i = 0;
  for (auto pos : positions) {
    state->animations.push_back({pos[0], pos[1], directions[i++], element, lv});
  }
}
