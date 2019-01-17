#include "HeuristicAI.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "AI.h"
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

tuple<MoveCommands*, AttackCommand*> HeuristicAI::getBestAction(
    Character* character) {
  // get our and enemy fighters
  shared_ptr<Fight> fight = state->getFight();
  vector<Character*> allies, ennemies;
  if (fight->getTeam(1)->getCharacter(character)) {
    allies = fight->getFightingCharacters(1);
    ennemies = fight->getFightingCharacters(0);
  } else {  // if (fight->getTeam(0)->getCharacter(character)) {
    allies = fight->getFightingCharacters(0);
    ennemies = fight->getFightingCharacters(1);
  }

  // get possible moves and attacks for the character
  vector<MoveCommands*> listmv;
  vector<AttackCommand*> listatk;
  // complete with no move and no atk
  listmv.push_back(nullptr);
  listatk.push_back(nullptr);

  for (auto pcommand : this->listCommands(character, 0)) {
    listmv.push_back(static_cast<MoveCommands*>(pcommand));
  }
  for (auto pcommand : this->listCommands(character, 1)) {
    listatk.push_back(static_cast<AttackCommand*>(pcommand));
  }

  Score score;
  score.setScoreAction(state, listmv[0], listatk[0], character, allies,
                       ennemies);
  vector<Score> scoresBest = {score};
  int scoreMax = scoresBest[0].getScore();
  // search best actions
  int i = 0, j = 0;
  vector<int> imax = {0}, jmax = {0};

  for (auto mv : listmv) {
    for (auto atk : listatk) {
      Score score;
      score.setScoreAction(state, mv, atk, character, allies, ennemies);

      if (score.getScore() > scoreMax) {
        imax.clear();
        jmax.clear();
        scoresBest.clear();
        scoreMax = score.getScore();
      }

      if (score.getScore() == scoreMax) {
        imax.push_back(i);
        jmax.push_back(j);
        scoresBest.push_back(score);
      }
      j++;
    }
    j = 0;
    i++;
  }

  // Choose one of the best actions randomly
  volatile int r = rand() % imax.size();
  i = imax[r];
  j = jmax[r];
  // Score scoreSelected = scoresBest[r];

  // cout << scoreSelected << endl;
  /*
  cout << "score action "
       << getScoreAction(listmv[i], listatk[j], *character, allies,
                         ennemies)
       << endl
       << endl;
   */
  return make_tuple(listmv[i], listatk[j]);
}

vector<Character*> HeuristicAI::getTurnOrder(vector<Character*> characters) {
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

void HeuristicAI::run(Character* character) {
  if (!state->isFighting())
    return;
  vector<Command*> listmv = this->listCommands(character, 0);
  vector<Command*> listatk = this->listCommands(character, 1);
  MoveCommands* mv;
  AttackCommand* atk;
  do {
    tuple<MoveCommands*, AttackCommand*> bestAction =
        this->getBestAction(character);
    mv = get<0>(bestAction);
    if (mv != nullptr) {
      engine->addCommand(mv);
      while (engine->getSize() != 0)
        ;
      listmv = this->listCommands(character, 0);
    }
    atk = get<1>(bestAction);
    if (atk != nullptr) {
      // atk->offsetPosition(mv->getDiff());
      engine->addCommand(atk);
      while (engine->getSize() != 0)
        ;
      listatk = this->listCommands(character, 1);
    }
  } while ((mv != nullptr || atk != nullptr) && state->isFighting() &&
           listmv.size() + listatk.size() > 2 && character->getPvCurrent() > 0);
}
