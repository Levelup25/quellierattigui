#include "Score.h"
#include <iostream>

using namespace std;
using namespace ai;
using namespace engine;
using namespace state;

int Score::getScore() const {
  int score = 0;
  score += bonusDmgEnnemy;
  score -= malusDmgAlly;
  score += bonusCloser;
  score -= malusPaUsed;
  score -= malusPmUsed;
  return score;
}

void Score::setScoreAction(MoveCommands* mv,
                           AttackCommand* atk,
                           Character* pcharacter,
                           vector<Character*> allies,
                           vector<Character*> ennemies) {
  // score atk
  if (atk != nullptr) {
    int bonusKillEnemy = 100;
    int MalusKillAlly = 200;
    int bonusHitEnemy = 10;
    int malusHitAlly = 5;
    Weapon* weapon = pcharacter->getWeapon();
    int dmg = weapon->getAbility(atk->getAbilityNumber())->getDamage();
    auto zoneDmg = atk->getZone(1);

    // score dmg
    for (auto coord : zoneDmg) {
      if (mv != nullptr) {
        coord[0] += mv->getDiff()[0];
        coord[1] += mv->getDiff()[1];
      }
      for (auto pennemy : ennemies) {
        if (pennemy->getI() == coord[0] && pennemy->getJ() == coord[1]) {
          int pv = pennemy->getPvCurrent();
          if (pv > 0) {
            int percent = (100 * dmg / pv);
            if (percent > 100) {
              int overkill = percent - 100;
              percent = 100;
              bonusDmgEnnemy += bonusKillEnemy;
              bonusDmgEnnemy -= overkill;
            }
            bonusDmgEnnemy += percent + bonusHitEnemy;
          }
        }
      }

      for (auto pally : allies) {
        if (pally->getI() == coord[0] && pally->getJ() == coord[1]) {
          int pv = pally->getPvCurrent();
          if (pv > 0) {
            int percent = (100 * dmg / pv);
            if (percent > 100) {
              percent = 100;
              malusDmgAlly += MalusKillAlly;
            }
            malusDmgAlly += percent + malusHitAlly;
          }
        }
      }
    }

    // score PA
    int malusRatioPaUsed = 1;
    malusPaUsed =
        malusRatioPaUsed * weapon->getAbility(atk->getAbilityNumber())->getPa();
  }

  // score PM
  if (mv != nullptr) {
    int bonusRatioCloserEnnemy = 2;
    int malusRatioPmUsed = 1;

    // bonus for getting closer
    for (auto pennemy : ennemies) {
      int ie = pennemy->getI(), je = pennemy->getJ();
      int io = pcharacter->getI(), jo = pcharacter->getJ();
      int in = io + mv->getDiff()[0], jn = jo + mv->getDiff()[1];
      int distanceOld = abs(io - ie) + abs(jo - je);
      int distanceNew = abs(in - ie) + abs(jn - je);
      int distanceDiff = distanceOld - distanceNew;
      bonusCloser += distanceDiff * bonusRatioCloserEnnemy;
    }
    int pmUsed = abs(mv->getDiff()[0]) + abs(mv->getDiff()[1]);
    malusPmUsed = pmUsed * malusRatioPmUsed;
  }
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