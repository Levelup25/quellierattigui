#include "Game.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <thread>
#include "ai/HeuristicAI.h"
#include "engine/FightCommand.h"
#include "engine/MoveCommand.h"

using namespace server;
using namespace state;
using namespace engine;
using namespace ai;
using namespace std;

Game::Game() {
  unsigned int seed = time(NULL);
  playerDB = new PlayerDB();
  state = new State(seed);
  engine = new Engine();
  ai = new HeuristicAI(state, engine);
}

PlayerDB* Game::getPlayerDB() {
  return playerDB;
}

State* Game::getState() {
  return state;
}

Engine* Game::getEngine() {
  return engine;
}

void Game::run() {
  bool end = false;
  while (!end) {
    for (int i = 0; i < (int)fights.size(); i++) {
      auto fight = fights[i];
      if (fight && fight->getTurn() == 0) {
        // int n = state->getN(), m = state->getM();
        // int xv0 =
        //         state->getTeams()[teamIndexes[i]]->getMainCharacter()->getI()
        //         / n,
        //     yv0 =
        //         state->getTeams()[teamIndexes[i]]->getMainCharacter()->getJ()
        //         / m;
        // int xv = xv0 * n, yv = yv0 * m, i, j;

        // for (auto chars : fight->getTeam(1)->getCharacters(fight->getNb())) {
        //   do {
        //     i = xv + n / 6 + rand() % (2 * n / 3);
        //     j = yv + m / 12 + rand() % (m / 4);
        //   } while (state->getCell(i, j)->getContent() != nothing);
        //   engine->addCommand(
        //       new MoveCommand(state, chars, chars->getI(), chars->getJ()));
        //   engine->addCommand(new MoveCommand(state, chars, i, j));
        //   state->getCell(i, j)->setContent(perso);
        // }
        // while (!end && engine->getSize())
        //   ;
      }

      if (fight && fight->getTurn() > 0) {
        vector<bool> aiToPlay;
        int id0 = -1, id1 = -1;
        Team* team0 = fight->getTeam(0);
        Team* team1 = fight->getTeam(1);
        vector<Team*> teams = state->getTeams();
        for (int i = 0; i < (int)teamIndexes.size(); i++) {
          if (teams[teamIndexes[i]] == team0) {
            id0 = i;
          } else if (teams[teamIndexes[i]] == team1) {
            id1 = i;
          }
        }
        if (id0 == -1)
          aiToPlay.push_back(false);
        if (id1 == -1)
          aiToPlay.push_back(true);

        for (bool i : aiToPlay) {
          if (engine->getSize() == 0 && fight && fight->getTurn() % 2 == (!i) &&
              fight->getTurn() != 0) {
            vector<Character*> vect =
                ai->getTurnOrder(fight->getFightingCharacters(i));
            for (auto c : vect) {
              ai->run(c);
            }
            if (fight->getFightingCharacters(i).size())
              engine->addCommand(new FightCommand(
                  state, engine, state->getFight()->getTeam(!i),
                  state->getFight()->getTeam(i)));
          }
        }
      }
    }
  }
}
