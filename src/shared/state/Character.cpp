#include "Character.h"

namespace state {

Character::Character(std::size_t i,
                     std::size_t j,
                     std::string nom,
                     CharacterType type,
                     int pv,
                     int pa,
                     int pm) {
  this->i = i;
  this->j = j;
  this->name = nom;
  this->type = type;
  this->pvMax = pv;
  this->paMax = pa;
  this->pmMax = pm;
}

// Getter
std::size_t Character::getI() {
  return i;
}

std::size_t Character::getJ() {
  return j;
}

// Setter
void Character::setI(std::size_t i) {
  this->i = i;
}
void Character::setJ(std::size_t j) {
  this->j = j;
}

}  // namespace state
