#include "Weapon.h"

using namespace std;
using namespace state;

Weapon::Weapon(ElementType element, std::string name, std::vector<Ability*> abilities) {
    this->element = element;
    this->name = name;
    this->abilities = abilities;
}

void Weapon::addAbility(Ability* ability) {
    abilities.push_back(ability);
}

void Weapon::delAbility(Ability* ability) {
    for (auto ab = abilities.begin(); ab != abilities.end(); ab++) {
        if (*ab == ability) abilities.erase(ab);
    }
}

vector<Ability*> Weapon::getAbilities() {
    return abilities;
}

string Weapon::getName() {
    return name;
}