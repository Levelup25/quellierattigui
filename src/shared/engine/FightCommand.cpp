#include "FightCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

FightCommand::FightCommand(State* state, Team* att, Team* def, bool reverse) {
  this->state = state;
  this->character = nullptr;
  this->att = att;
  this->def = def;
  this->reverse = reverse;
  if (state->getFight()) {
    for (auto c : state->getFight()->getFightingCharacters(
             (state->getFight()->getTurn() + 1) % 2)) {
      pa.push_back(c->getPaCurrent() - c->getPaMax());
      pm.push_back(c->getPmCurrent() - c->getPmMax());
    }
  }
}

void FightCommand::execute() {
  if (!reverse) {
    if (!state->isFighting() && att != def) {
      shared_ptr<Fight> fight(new Fight(att, def, 3));
      state->setFight(fight);
      for (auto c : fight->getTeam(0)->getCharacters(fight->getNb())) {
        c->resetPm();
        c->resetPa();
        c->resetPv();
      }

      Team* team1 = fight->getTeam(0);
      Team* team2 = fight->getTeam(1);
      int i0 = team2->getMainCharacter()->getI(),
          j0 = team2->getMainCharacter()->getJ(), n = state->getN(),
          m = state->getM();
      int xv = (i0 / n) * n, yv = (j0 / m) * m;

      if (state->isFighting()) {
        int i, j, nb = fight->getNb();
        Character* maincharacter = team1->getMainCharacter();
        do {
          i = xv + n / 6 + rand() % (2 * n / 3);
          j = yv + 2 * m / 3 + rand() % (m / 4);
        } while (state->getCell(i, j)->getContent() != nothing);
        state->moveCharacter(maincharacter, i, j);
        maincharacter->setDirection(north);

        for (auto oppchars : team2->getCharacters(nb)) {
          do {
            i = xv + n / 6 + rand() % (2 * n / 3);
            j = yv + m / 12 + rand() % (m / 4);
          } while (state->getCell(i, j)->getContent() != nothing);
          state->moveCharacter(oppchars, i, j);
          oppchars->setDirection(south);
        }
      }
      // 2/12   8/12   2/12   =>  1/6     2/3     1/6

      // 1/12 => 1/12
      // 3/12 => 1/4
      // 4/12 => 1/3
      // 3/12 => 1/4
      // 1/12 => 1/12
    } else if (state->isFighting()) {
      int i = 0;
      for (auto c : state->getFight()->getFightingCharacters(
               (state->getFight()->getTurn() + 1) % 2)) {
        c->removePa(pa[i]);
        c->removePm(pm[i++]);
      }
      state->getFight()->addTurn();
      state->etatCombat = 0;
    }
  } else {
    if (state->getFight()) {
      int i = 0;
      state->getFight()->addTurn(-1);
      for (auto c : state->getFight()->getFightingCharacters(
               (state->getFight()->getTurn() + 1) % 2)) {
        c->removePa(-pa[i]);
        c->removePm(-pm[i++]);
      }
      state->etatCombat = 0;
      state->endFight();
    }
  }
}