#include "Relatif.h"
#include <iostream>

using namespace render;
using namespace std;

Relatif::Relatif() {}

Relatif::Relatif(float pixel) {
  *this = pixel;
}

Relatif::Relatif(std::string str) {
  *this = str;
}

void Relatif::operator=(float pixel) {
  this->pixel = pixel;
  computeMethod = ComputeMethodType::pixel;
}

void Relatif::operator=(std::string str) {
  if (str.size() > 1) {
    auto last = str.substr(str.size() - 1);
    auto numberStr = str.substr(0, str.size() - 1);
    if (last == "%") {
      computeMethod = ComputeMethodType::percent;
      percent = (float)atof(numberStr.c_str());
      return;
    }
  }

  else if (str.size() == 1) {
    string usablechar = "lrtbm";
    if (usablechar.find(str) != std::string::npos) {
      computeMethod = ComputeMethodType::alignement;
      alignement = str;
      return;
    }
  }
}

ComputeMethodType Relatif::getComputeMethod() const {
  return computeMethod;
}

float Relatif::getPixel() const {
  return pixel;
}

float Relatif::getPercent() const {
  return percent;
}

std::string Relatif::getAlignement() const {
  return alignement;
}

Relatif::operator string() const {
  switch (computeMethod) {
    case ComputeMethodType::pixel:
      return to_string((int)pixel);

    case ComputeMethodType::percent:
      return to_string((int)percent) + "%";

    case ComputeMethodType::alignement:
      return alignement;

    default:
      return "?";
  }
}
