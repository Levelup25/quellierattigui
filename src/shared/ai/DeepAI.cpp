#include "DeepAI.h"
#include <algorithm>
#include <iostream>
#include "engine/AttackCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

DeepAI::DeepAI(State* state, Engine* engine) {
  this->state = state;
  this->engine = engine;
}

// bool DeepAI::isCharacterAtpos(Character* c, int i, int j) {
//   // vector<Character*> fighters =
//   state->getFight()->getFightingCharacters();
//   // if (find(fighters.begin(), fighters.end(), c) != fighters.end())
//   return c->getI() == i && c->getJ() == j;
//   // return false;
// }

// Score DeepAI::getScoreAction(MoveCommands* mv,
//                              AttackCommand* atk,
//                              Character* pcharacter,
//                              vector<Character*> allies,
//                              vector<Character*> ennemies) {
//   Score score;

//   // score atk
//   if (atk != nullptr) {
//     int bonusKillEnemy = 100;
//     int MalusKillAlly = 200;
//     int bonusHitEnemy = 10;
//     int malusHitAlly = 5;
//     Weapon* weapon = pcharacter->getWeapon();
//     int dmg = weapon->getAbility(atk->getAbilityNumber())->getDamage();
//     auto zoneDmg = atk->getZone(1);

//     // score dmg
//     for (auto coord : zoneDmg) {
//       if (mv != nullptr) {
//         coord[0] += mv->getDiff()[0];
//         coord[1] += mv->getDiff()[1];
//       }
//       for (auto pennemy : ennemies) {
//         if (isCharacterAtpos(pennemy, coord[0], coord[1])) {
//           int pv = pennemy->getPvCurrent();
//           int percent = (100 * dmg / pv);
//           if (percent > 100) {
//             int overkill = percent - 100;
//             percent = 100;
//             score.bonusDmgEnnemy += bonusKillEnemy;
//             score.bonusDmgEnnemy -= overkill;
//           }
//           score.bonusDmgEnnemy += percent + bonusHitEnemy;
//         }
//       }

//       for (auto pally : allies) {
//         if (isCharacterAtpos(pally, coord[0], coord[1])) {
//           int pv = pally->getPvCurrent();
//           int percent = (100 * dmg / pv);
//           if (percent > 100) {
//             percent = 100;
//             score.malusDmgAlly += MalusKillAlly;
//           }
//           score.malusDmgAlly += percent + malusHitAlly;
//         }
//       }
//     }

//     // score PA
//     int malusRatioPaUsed = 1;
//     score.malusPaUsed =
//         malusRatioPaUsed *
//         weapon->getAbility(atk->getAbilityNumber())->getPa();
//   }

//   // score PM
//   if (mv != nullptr) {
//     int bonusRatioCloserEnnemy = 2;
//     int malusRatioPmUsed = 1;

//     // bonus for getting closer
//     for (auto pennemy : ennemies) {
//       int ie = pennemy->getI(), je = pennemy->getJ();
//       int io = pcharacter->getI(), jo = pcharacter->getJ();
//       int in = io + mv->getDiff()[0], jn = jo + mv->getDiff()[1];
//       int distanceOld = abs(io - ie) + abs(jo - je);
//       int distanceNew = abs(in - ie) + abs(jn - je);
//       int distanceDiff = distanceOld - distanceNew;
//       score.bonusCloser += distanceDiff * bonusRatioCloserEnnemy;
//     }
//     int pmUsed = abs(mv->getDiff()[0]) + abs(mv->getDiff()[1]);
//     score.malusPmUsed = pmUsed * malusRatioPmUsed;
//   }
//   return score;
// }

// std::tuple<MoveCommands*, AttackCommand*> DeepAI::getBestAction(
//     Character* character) {
//   // get our and enemy fighters
//   vector<Character*> allies = state->getFight()->getFightingCharacters(1);
//   vector<Character*> ennemies = state->getFight()->getFightingCharacters(0);

//   // get possible moves and attacks for the character
//   vector<MoveCommands*> listmv;
//   vector<AttackCommand*> listatk;
//   // complete with no move and no atk
//   listmv.push_back(nullptr);
//   listatk.push_back(nullptr);

//   for (auto pcommand : this->listCommands(character, 0)) {
//     listmv.push_back(static_cast<MoveCommands*>(pcommand));
//   }
//   for (auto pcommand : this->listCommands(character, 1)) {
//     listatk.push_back(static_cast<AttackCommand*>(pcommand));
//   }

//   std::vector<Score> scoresBest = {
//       getScoreAction(listmv[0], listatk[0], character, allies, ennemies)};
//   int scoreMax = scoresBest[0].getScore();
//   // search best actions
//   int i = 0, j = 0;
//   vector<int> imax = {0}, jmax = {0};

//   for (auto mv : listmv) {
//     for (auto atk : listatk) {
//       Score score = getScoreAction(mv, atk, character, allies, ennemies);

//       if (score.getScore() > scoreMax) {
//         imax.clear();
//         jmax.clear();
//         scoresBest.clear();
//         scoreMax = score.getScore();
//       }

//       if (score.getScore() == scoreMax) {
//         imax.push_back(i);
//         jmax.push_back(j);
//         scoresBest.push_back(score);
//       }
//       j++;
//     }
//     j = 0;
//     i++;
//   }

//   // Choose one of the best actions randomly
//   volatile int r = rand() % imax.size();
//   i = imax[r];
//   j = jmax[r];
//   // Score scoreSelected = scoresBest[r];

//   // cout << scoreSelected << endl;
//   /*
//   cout << "score action "
//        << getScoreAction(listmv[i], listatk[j], *character, allies,
//                          ennemies)
//        << endl
//        << endl;
//    */
//   return std::make_tuple(listmv[i], listatk[j]);
// }

vector<Character*> DeepAI::getTurnOrder(vector<Character*> characters) {
  vector<Character*> v;
  vector<int> dist;
  for (auto c : characters) {
    int d = 0;
    for (auto c2 : state->getFight()->getFightingCharacters(0))
      d += abs(c->getI() - c2->getI()) + abs(c->getJ() - c2->getJ());
    dist.push_back(d);
  }

  int max =
      dist[distance(dist.begin(), max_element(dist.begin(), dist.end()))] + 1;
  for (int j = 0; j < (int)characters.size(); j++) {
    int i = distance(dist.begin(), min_element(dist.begin(), dist.end()));
    v.push_back(characters[i]);
    dist[i] = max;
  }
  return v;
}

void DeepAI::run(Character* character) {
  vector<Command*> commands = this->listCommands(character);
  while (state->isFighting() && commands.size() > 0 &&
         character->getPvCurrent() > 0) {
    engine->addCommand(commands[rand() % commands.size()]);
    while (engine->getSize() != 0)
      ;
    commands = this->listCommands(character);
  }
  //   vector<Command*> listmv = this->listCommands(character, 0);
  //   vector<Command*> listatk = this->listCommands(character, 1);
  //   MoveCommands* mv;
  //   AttackCommand* atk;
  //   do {
  //     std::tuple<MoveCommands*, AttackCommand*> bestAction =
  //         this->getBestAction(character);
  //     mv = std::get<0>(bestAction);
  //     if (mv != nullptr) {
  //       engine->addCommand(mv);
  //       while (engine->getSize() != 0)
  //         ;
  //       listmv = this->listCommands(character, 0);
  //     }
  //     atk = std::get<1>(bestAction);
  //     if (atk != nullptr) {
  //       engine->addCommand(atk);
  //       while (engine->getSize() != 0)
  //         ;
  //       listatk = this->listCommands(character, 1);
  //     }
  //   } while ((mv != nullptr || atk != nullptr) && state->isFighting() &&
  //            listmv.size() + listatk.size() > 2 && character->getPvCurrent()
  //            > 0);
}
