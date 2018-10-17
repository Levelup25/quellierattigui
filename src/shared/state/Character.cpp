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
