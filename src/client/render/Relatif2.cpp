#include "Relatif2.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace render;
using namespace std;

Relatif2::Relatif2() {}
Relatif2::Relatif2(Relatif x, Relatif y) {
  this->x = x;
  this->y = y;
}
Relatif2::Relatif2(sf::Vector2f pos) {
  this->x = pos.x;
  this->y = pos.y;
}

namespace render {
std::ostream& operator<<(std::ostream& os, const Relatif2& rel2) {
  os << "(";
  os << std::setw(3) << rel2.x;
  os << ", ";
  os << std::setw(3) << rel2.y;
  os << ")";
  return os;
}
}  // namespace render