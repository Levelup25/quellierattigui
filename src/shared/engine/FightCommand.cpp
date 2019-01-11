#include "FightCommand.h"
#include <iostream>
#include "MoveCommand.h"

using namespace std;
using namespace state;
using namespace engine;

FightCommand::FightCommand(State* state,
                           Engine* engine,
                           Team* att,
                           Team* def,
                           bool reverse) {
  type = "FightCommand";
  this->state = state;
  this->engine = engine;
  this->character = nullptr;
  this->att = att;
  this->def = def;
  this->reverse = reverse;
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

      if (state->isFighting() && engine->getSize() == 1) {
        int i, j, nb = fight->getNb();
        Character* maincharacter = team1->getMainCharacter();
        // if (!(maincharacter->getI() >= xv + n / 6 &&
        //       maincharacter->getI() < xv + n / 6 + 2 * n / 3 &&
        //       maincharacter->getJ() >= yv + 2 * m / 3 &&
        //       maincharacter->getJ() < yv + 2 * m / 3 + m / 4)) {
        do {
          i = xv + n / 6 + rand() % (2 * n / 3);
          j = yv + 2 * m / 3 + rand() % (m / 4);
        } while (state->getCell(i, j)->getContent() != nothing);
        engine->addCommand(new MoveCommand(state, maincharacter,
                                           maincharacter->getI(),
                                           maincharacter->getJ()));
        engine->addCommand(new MoveCommand(state, maincharacter, i, j));
        // state->moveCharacter(maincharacter, i, j);
        // maincharacter->setDirection(north);

        for (auto oppchars : team2->getCharacters(nb)) {
          // if (!(oppchars->getI() >= xv + n / 6 &&
          //       oppchars->getI() < xv + n / 6 + 2 * n / 3 &&
          //       oppchars->getJ() >= yv + m / 12 &&
          //       oppchars->getJ() < yv + m / 12 + m / 4)) {
          do {
            i = xv + n / 6 + rand() % (2 * n / 3);
            j = yv + m / 12 + rand() % (m / 4);
          } while (state->getCell(i, j)->getContent() != nothing);
          engine->addCommand(new MoveCommand(state, oppchars, oppchars->getI(),
                                             oppchars->getJ()));
          engine->addCommand(new MoveCommand(state, oppchars, i, j));
          state->getCell(i, j)->setContent(perso);
          // state->moveCharacter(oppchars, i, j);
          // oppchars->setDirection(south);
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
        if (c->getPvCurrent() > c->getPvMax())
          pv.push_back(c->getPvCurrent() - c->getPvMax());
        else
          pv.push_back(0);
        pa.push_back(c->getPaCurrent() - c->getPaMax());
        pm.push_back(c->getPmCurrent() - c->getPmMax());
        c->removePv(pv[i]);
        c->removePa(pa[i]);
        c->removePm(pm[i++]);
      }
      state->getFight()->addTurn();
      state->etatCombat = 0;
    }
  } else {  // reverse
    if (state->getFight()) {
      int i = 0;
      state->getFight()->addTurn(-1);
      for (auto c : state->getFight()->getFightingCharacters(
               (state->getFight()->getTurn() + 1) % 2)) {
        c->removePv(-pv[i]);
        c->removePa(-pa[i]);
        c->removePm(-pm[i++]);
      }
      state->etatCombat = 0;
      state->endFight();
    }
  }
}

void const FightCommand::serialize(Json::Value& out) {
  out["command"] = "FightCommand";
  vector<Team*> teams = state->getTeams();
  int k = teams.size();
  while (k--)
    if (att == teams[k]) {
      out["att"] = k;
      break;
    }
  k = teams.size();
  while (k--)
    if (def == teams[k]) {
      out["def"] = k;
      break;
    }
}

FightCommand* FightCommand::deserialize(const Json::Value& in,
                                        State* state,
                                        Engine* engine) {
  Team* att = state->getTeams()[in.get("att", 0).asInt()];
  Team* def = state->getTeams()[in.get("def", 0).asInt()];
  return new FightCommand(state, engine, att, def);
}