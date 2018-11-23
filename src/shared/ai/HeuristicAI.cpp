#include "HeuristicAI.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine/AttackCommand.h"
#include "engine/MoveCommands.h"

using namespace std;
using namespace state;
using namespace engine;
using namespace ai;

HeuristicAI::HeuristicAI(State* state, Engine* engine) {
  this->state = state;
  this->engine = engine;
}

bool isCharacterAtpos(Character c, int i, int j) {
  return c.getI() == i && c.getJ() == j;
}

int getScoreAction(MoveCommands* mv,
                   AttackCommand* atk,
                   Character character,
                   vector<Character*> myFighters,
                   vector<Character*> theirFighters) {
  int score = 0;

  if (atk == nullptr)
    return 0;
  Weapon weapon = *(character.getWeapon());
  int dmg = weapon.getAbility(atk->getAbilityNumber())->getDamage();
  auto zoneDmg = atk->getZone(1, false);

  for (auto coord : zoneDmg) {
    if (mv != nullptr) {
      coord[0] += mv->getDiff()[0];
      coord[1] += mv->getDiff()[1];
    }
    for (auto pcharacter : myFighters) {
      if (isCharacterAtpos(*pcharacter, coord[0], coord[1])) {
        int pv = pcharacter->getPvCurrent();
        score += dmg < pv ? dmg : pv + 1;
      }
    }
    for (auto pcharacter : theirFighters) {
      if (isCharacterAtpos(*pcharacter, coord[0], coord[1])) {
        int pv = pcharacter->getPvCurrent();
        score -= dmg < pv ? dmg : pv + 1;
      }
    }
    int paCost = weapon.getAbility(atk->getAbilityNumber())->getPa();
    // score -= paCost;
  }
  return score;
}

std::tuple<MoveCommands*, AttackCommand*> HeuristicAI::getBestAction(
    Character* character) {
  // get our and enemy fighters
  vector<Character*> myFighters = state->getFight()->getFightingCharacters(0);
  vector<Character*> theirFighters =
      state->getFight()->getFightingCharacters(1);

  // get possible move and attack for character
  vector<MoveCommands*> listmv;
  vector<AttackCommand*> listatk;
  for (auto pcommand : this->listCommands(character, 0)) {
    listmv.push_back(static_cast<MoveCommands*>(pcommand));
  }
  for (auto pcommand : this->listCommands(character, 1)) {
    listatk.push_back(static_cast<AttackCommand*>(pcommand));
  }

  // complete avec no move et no atk
  listmv.insert(listmv.begin(), nullptr);
  listatk.insert(listatk.begin(), nullptr);

  int scoreMax = getScoreAction(listmv[0], listatk[0], *character, myFighters,
                                theirFighters);
  // search best actions
  int i = 0, j = 0;
  vector<int> imax = {0}, jmax = {0};

  // out << "score = {";
  for (auto mv : listmv) {
    for (auto atk : listatk) {
      int score =
          getScoreAction(mv, atk, *character, myFighters, theirFighters);
      // cout << score << ", ";

      if (score > scoreMax) {
        imax.clear();
        jmax.clear();
        scoreMax = score;
      }

      if (score == scoreMax) {
        imax.push_back(i);
        jmax.push_back(j);
      }
      j++;
    }
    j = 0;
    i++;
  }
  // cout << "}" << endl;
  // cout << "scoreMax = " << scoreMax << endl;
  // Choose one of the best action
  int r = rand() % imax.size();
  i = imax[r];
  j = jmax[r];
  /*
  cout << "score action "
       << getScoreAction(listmv[i], listatk[j], *character, myFighters,
                         theirFighters)
       << endl
       << endl;
       */
  return std::make_tuple(listmv[i], listatk[j]);
}

void HeuristicAI::run(Character* character) {
  while (engine->getSize() != 0)
    ;
  vector<Command*> listmv = this->listCommands(character, 0);
  vector<Command*> listatk = this->listCommands(character, 1);

  std::tuple<MoveCommands*, AttackCommand*> bestAction =
      this->getBestAction(character);
  MoveCommands* mv = std::get<0>(bestAction);

  if (mv != nullptr)
    engine->addCommand(mv);

  while (engine->getSize() > 0) {
  }

  bestAction = this->getBestAction(character);
  AttackCommand* atk = std::get<1>(bestAction);

  if (atk != nullptr)
    engine->addCommand(atk);
}
