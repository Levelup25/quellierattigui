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
Relatif2::operator string() const {
  std::stringstream stream;
  stream << "(";
  stream << std::setw(3) << (string)x;
  stream << ", ";
  stream << std::setw(3) << (string)y;
  stream << ")";
  return stream.str();
}