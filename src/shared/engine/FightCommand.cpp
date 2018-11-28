#include "FightCommand.h"
#include <iostream>

using namespace std;
using namespace state;
using namespace engine;

FightCommand::FightCommand(State* state, Team* att, Team* def) {
  this->state = state;
  this->character = nullptr;
  this->att = att;
  this->def = def;
}

void FightCommand::execute() {
  if (!state->isFighting() && att != nullptr && def != nullptr) {
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
      for (auto mainchars : team1->getCharacters(nb)) {
        do {
          i = xv + n / 6 + rand() % (2 * n / 3);
          j = yv + 2 * m / 3 + rand() % (m / 4);
        } while (state->getCell(i, j)->getContent() != nothing);
        state->moveCharacter(mainchars, i, j);
        mainchars->setDirection(north);
      }
      for (auto oppchars : team2->getCharacters(nb)) {
        do {
          i = xv + n / 6 + rand() % (2 * n / 3);
          j = yv + m / 12 + rand() % (m / 4);
        } while (state->getCell(i, j)->getContent() != nothing);
        state->moveCharacter(oppchars, i, j);
        oppchars->setDirection(south);
      }
    }
    state->resetContents();
    // 2/12   8/12   2/12   =>  1/6     2/3     1/6

    // 1/12 => 1/12
    // 3/12 => 1/4
    // 4/12 => 1/3
    // 3/12 => 1/4
    // 1/12 => 1/12
  } else {
    state->getFight()->endTurn();
    state->endFight();
    state->etatCombat = 0;
  }
}