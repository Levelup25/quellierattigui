#include "AttackCommand.h"
#include <iostream>
#include "DamageCommand.h"
#include "FightCommand.h"

using namespace std;
using namespace state;
using namespace engine;

AttackCommand::AttackCommand(State* state,
                             Engine* engine,
                             Character* character,
                             vector<int> position,
                             int abilityNumber,
                             bool reverse) {
  this->state = state;
  this->engine = engine;
  this->character = character;
  this->abilityNumber = abilityNumber;
  this->position = position;
  this->reverse = reverse;
  //    position[0] += character->getI();
  //    position[1] += character->getJ();
}

int AttackCommand::getAbilityNumber() {
  return abilityNumber;
}

// in absolute pos (not relative to character or view)

void AttackCommand::setZones(bool cut) {
  int n = state->getN(), m = state->getM();
  ability = character->getWeapon()->getAbility(abilityNumber);
  if (ability->getPa() <= character->getPaCurrent()) {
    int i0 = character->getI(), j0 = character->getJ();
    int i = i0 + position[0], j = j0 + position[1];
    targets = ability->getTargetZone({i0, j0});
    if (cut) {
      for (auto target = targets.end() - 1; target >= targets.begin();
           target--) {
        if ((*target)[0] / n != i0 / n || (*target)[0] < 0 ||
            (*target)[1] / m != j0 / m || (*target)[1] < 0)
          targets.erase(target);
      }
    }
    for (auto coord : targets) {
      if (coord[0] == i && coord[1] == j) {
        effects = ability->getEffectZone({i, j});
        if (cut) {
          for (auto effect = effects.end() - 1; effect >= effects.begin();
               effect--) {
            if ((*effect)[0] / n != i / n || (*effect)[0] < 0 ||
                (*effect)[1] / m != j / m || (*effect)[1] < 0)
              effects.erase(effect);
          }
        }
        return;
      }
    }
    effects = {};
  } else {
    targets = {};
    effects = {};
  }
}

vector<vector<int>> AttackCommand::getZone(size_t i, bool cut) {
  this->setZones(cut);
  if (i == 0)
    return targets;
  else if (i == 1)
    return effects;
  return {};
}

void AttackCommand::execute() {
  if (reverse) {
    character->removePa(-ability->getPa());
    engine->addCommand(nullptr);
  } else {
    this->setZones();
    if (effects.size() > 0 && ability->getPa() <= character->getPaCurrent()) {
      character->removePa(ability->getPa());

      int I = position[0], J = position[1];
      int direction;
      if (I > 0 && J == 0)
        direction = 0;
      else if (I == 0 && J < 0)
        direction = 1;
      else if (I < 0 && J == 0)
        direction = 2;
      else if (I == 0 && J > 0)
        direction = 3;
      else if (I > 0 && J > 0)
        direction = 4;
      else if (I < 0 && J > 0)
        direction = 5;
      else if (I < 0 && J < 0)
        direction = 6;
      else if (I > 0 && J < 0)
        direction = 7;
      else
        direction = 1;

      vector<vector<int>> v;
      vector<int> v2{direction};

      engine->addCommand(new DamageCommand(
          state, engine, v, v2, ability->getElement(), ability->getLv()));

      float nb = max(abs(I), abs(J));
      for (int i = 1; i <= nb; i++) {
        v = {{(int)(character->getI() + i * I / nb),
              (int)(character->getJ() + i * J / nb)}};
        for (int i = 0; i < 5; i++)
          engine->addCommand(new DamageCommand(
              state, engine, v, v2, ability->getElement(), ability->getLv()));
      }

      int dist = 1;
      for (auto effect : effects) {
        effect[0] -= character->getI();
        effect[1] -= character->getJ();
        if (effect != position) {
          int I = effect[0] - position[0], J = effect[1] - position[1];
          if (abs(I) + abs(J) > dist) {
            dist = abs(I) + abs(J);
            engine->addCommand(new DamageCommand(
                state, engine, v, v2, ability->getElement(), ability->getLv()));
          }
          if (I > 0 && J == 0)
            direction = 0;
          else if (I == 0 && J < 0)
            direction = 1;
          else if (I < 0 && J == 0)
            direction = 2;
          else if (I == 0 && J > 0)
            direction = 3;
          else if (I > 0 && J > 0)
            direction = 4;
          else if (I < 0 && J > 0)
            direction = 5;
          else if (I < 0 && J < 0)
            direction = 6;
          else if (I > 0 && J < 0)
            direction = 7;
          else
            direction = 1;
          effect[0] += character->getI();
          effect[1] += character->getJ();
          v.push_back(effect);
          v2.push_back(direction);
        }
      }
      for (int i = 0; i < 25; i++)
        engine->addCommand(new DamageCommand(
            state, engine, v, v2, ability->getElement(), ability->getLv()));

      engine->addCommand(
          new DamageCommand(state, engine, v, v2, ability->getElement(),
                            ability->getLv(), ability->getDamage(), reverse));

      v.clear();
      engine->addCommand(new DamageCommand(
          state, engine, v, v2, ability->getElement(), ability->getLv()));
    }
  }
}