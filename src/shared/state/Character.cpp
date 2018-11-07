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
    weapon = new Weapon();
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

void Character::setDirection(int direction) {
    this->direction = (Direction) direction;
}

float Character::getI() {
    return i;
}

float Character::getJ() {
    return j;
}

void Character::setI(float i) {
    this->i = i;
}

void Character::setJ(float j) {
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

void Character::setPv(unsigned int pv) {
    pvMax = pv;
}

void Character::setPm(unsigned int pm) {
    pmMax = pm;
}

void Character::setPa(unsigned int pa) {
    paMax = pa;
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

void Character::resetPv() {
    pvCurrent = pvMax;
}

void Character::resetPm() {
    pmCurrent = pmMax;
}

void Character::resetPa() {
    paCurrent = paMax;
}

void Character::removePv(int pv) {
    if (pv >= pvCurrent) pvCurrent = 0;
    else pvCurrent -= pv;
}

void Character::removePm(int pm) {
    if (pm >= pmCurrent) pmCurrent = 0;
    else pmCurrent -= pm;
}

void Character::removePa(int pa) {
    if (pa >= paCurrent) paCurrent = 0;
    else paCurrent -= pa;
}