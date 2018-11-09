#include "Relatif.h"
#include <iostream>

using namespace render;
using namespace std;

Relatif::Relatif() {}

Relatif::Relatif(int offset) {
  *this = offset;
}

Relatif::Relatif(float offset) {
  *this = offset;
}

Relatif::Relatif(std::string str) {
  *this = str;
}

void Relatif::operator=(int offset) {
  this->offset = offset;
  computeMethod = 0;
}

void Relatif::operator=(float offset) {
  this->offset = offset;
  computeMethod = 0;
}

void Relatif::operator=(std::string str) {}
