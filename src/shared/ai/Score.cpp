#include "Score.h"
#include <iostream>

using namespace ai;

int Score::getScore() {
  int score = 0;
  score += bonusDmgEnnemy;
  score -= malusDmgAlly;
  score += bonusCloser;
  score -= malusPaUsed;
  score -= malusPmUsed;
  return score;
}
