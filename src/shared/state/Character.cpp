#include "Character.h"

using namespace std;
using namespace state;

Character::Character(int id,
                     string nom,
                     CharacterType type,
                     int pv,
                     int pa,
                     int pm) {
  // FIXME : Remplacer i et j par autre chose
  this->i = 0;
  this->j = 0;
  this->id = id;
  this->direction = (Direction)0;
  this->name = nom;
  if (!name.compare("")) {
    vector<string> names = {"Goku",   "Snake",  "Cloud",    "Samus",
                            "Mario",  "Lara",   "Link",     "Kratos",
                            "Dante",  "Altair", "Ryu",      "Sacha",
                            "Jotaro", "Yugi",   "Meliodas", "Kirito"};
    this->name = names[rand() % names.size()];
  }
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
  this->direction = (Direction)direction;
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

Weapon* Character::getWeapon() {
  return weapon;
}

void Character::setWeapon(Weapon* weapon) {
  this->weapon = weapon;
}

void Character::removeWeapon() {
  this->weapon = nullptr;
}

void Character::setPv(int pv) {
  pvMax = pv;
  pvCurrent = pv;
}

void Character::setPm(int pm) {
  pmMax = pm;
  pmCurrent = pm;
}

void Character::setPa(int pa) {
  paMax = pa;
  paCurrent = pa;
}

int Character::getPvCurrent() {
  return pvCurrent;
}

int Character::getPmCurrent() {
  return pmCurrent;
}

int Character::getPaCurrent() {
  return paCurrent;
}

int Character::getPvMax() {
  return pvMax;
}

int Character::getPmMax() {
  return pmMax;
}

int Character::getPaMax() {
  return paMax;
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
  // if (pv >= pvCurrent)
  //   pvCurrent = 0;
  // else
  pvCurrent -= pv;
}

void Character::removePm(int pm) {
  // if (pm >= pmCurrent)
  //   pmCurrent = 0;
  // else
  pmCurrent -= pm;
}

void Character::removePa(int pa) {
  // if (pa >= paCurrent)
  //   paCurrent = 0;
  // else
  paCurrent -= pa;
}