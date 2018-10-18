#include "Character.h"

using namespace std;
using namespace state;

Character::Character(size_t i, size_t j, string nom, CharacterType type, int pv, int pa, int pm) {
    this->i = i;
    this->j = j;
    this->name = nom;
    this->type = type;
    this->pvMax = pv;
    this->paMax = pa;
    this->pmMax = pm;
}

size_t Character::getI() {
    return i;
}

size_t Character::getJ() {
    return j;
}

void Character::setI(size_t i) {
    this->i = i;
}

void Character::setJ(size_t j) {
    this->j = j;
}

string Character::getName() {
    return name;
}

void Character::setName(string name) {
    this->name = name;
}

CharacterType Character::getType() {
    return type;
}

void Character::setType() {
    this->type = type;
}

Weapon* Character::getWeapon() {
    return weapon;
}

void Character::setWeapon(Weapon* weapon) {
    this->weapon = weapon;
}

void Character::removeWeapon() {
    this->weapon = nullptr;
}

void Character::getDamage(Ability* ability) {
    int dmg = ability->getDamage();
    if (dmg > pvCurrent) pvCurrent = 0;
    else pvCurrent -= ability->getDamage();
    if (pvCurrent > pvMax) pvCurrent = pvMax;
}

unsigned int Character::getPv() {
    return pvCurrent;
}