#include "Weapon.h"

using namespace state;

Weapon::Weapon(ElementType element, std::vector<Ability*> abilities) {
  this->element = element;
  this->abilities = abilities;
}

void Weapon::addAbility(Ability& ability) {
  abilities.push_back(&ability);
}

std::vector<Ability*> Weapon::getAbilities() {
  return abilities;
}

void Weapon::delAbility(Ability* pAbility) {
  for (auto it = abilities.begin(); it != abilities.end(); it++) {
    Ability* pa = *it;
    if (pa == pAbility) {
      abilities.erase(it);
      return;
    }
  }
}