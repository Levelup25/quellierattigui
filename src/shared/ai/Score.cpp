#include "Score.h"
#include <iostream>

using namespace ai;
using namespace std;

int Score::getScore() const {
  int score = 0;
  score += bonusDmgEnnemy;
  score -= malusDmgAlly;
  score += bonusCloser;
  score -= malusPaUsed;
  score -= malusPmUsed;
  return score;
}

namespace ai {
std::ostream& operator<<(std::ostream& os, const ai::Score& score) {
  os << "score = " << score.getScore() << ", ";
  os << "(";
  os << "bonusDmgEnnemy = " << score.bonusDmgEnnemy << ", ";
  os << "malusDmgAlly = " << score.malusDmgAlly << ", ";
  os << "bonusCloser = " << score.bonusCloser << ", ";
  os << "malusPaUsed = " << score.malusPaUsed << ", ";
  os << "malusPmUsed = " << score.malusPmUsed;
  os << ")";
  return os;
}
}  // namespace ai