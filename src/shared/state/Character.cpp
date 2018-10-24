#include "Character.h"

using namespace std;
using namespace state;

Character::Character(int id, string nom, CharacterType type, int pv, int pa, int pm) {
    // FIXME : Remplacer i et j par autre chose
    this->i = 0;
    this->j = 0;
    this->id = id;
    this->direction = (Direction) 0;
    this->name = nom;
    this->type = type;
    this->pvMax = pv;
    this->pvCurrent = pv;
    this->paMax = pa;
    this->paCurrent = pa;
    this->pmMax = pm;
    this->pmCurrent = pm;
}

int Character::getId() {
    return id;
}

void Character::setId(int id) {
    this->id = id;
}

Direction Character::getDirection() {
    return direction;
}

void Character::setDirection(Direction direction) {
    this->direction = direction;
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

Weapon *Character::getWeapon() {
    return weapon;
}

void Character::setWeapon(Weapon *weapon) {
    this->weapon = weapon;
}

void Character::removeWeapon() {
    this->weapon = nullptr;
}

unsigned int Character::getPv() {
    return pvCurrent;
}

unsigned int Character::getPm() {
    return pmCurrent;
}

unsigned int Character::getPa() {
    return paCurrent;
}

void Character::removePv(int pv) {
    pvCurrent -= pv;
}

void Character::removePm(int pm) {
    pmCurrent -= pm;
}

void Character::removePa(int pa) {
    paCurrent -= pa;
}