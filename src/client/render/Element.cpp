#include "Element.h"
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace render;
using namespace std;

Element::Element() {
  posRelative = {0, 0};
  sizeRelative = {10, 10};
};

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto pchild : pchildren) {
    target.draw(*pchild);
  }
};

void Element::reactEvent(sf::Event event, sf::Vector2f posMouse){};

const Relatif2 Element::getPosRelative() const {
  return posRelative;
}
void Element::setPosRelative(const Relatif2 pos) {
  this->posRelative = pos;
  updatePosAbs();
}

const sf::Vector2f Element::getPosAbs() const {
  return posAbsCache;
}

const sf::Vector2f Element::getSizeAbs() const {
  return sizeAbsCache;
}

void Element::setSizeRelative(const Relatif2 sizeRelative) {
  this->sizeRelative = sizeRelative;
  updateSizeAbs();
}

void Element::add(Element* pchild) {
  pchildren.push_back(pchild);
  pchild->setParent(this);
}

void Element::remove(Element* pchild) {
  for (auto it = pchildren.begin(); it != pchildren.end(); it++) {
    auto p = *it;
    if (p == pchild) {
      pchildren.erase(it);
      return;
    }
  }
}

void Element::notifyEditPosAbs() {
  for (auto pchild : pchildren) {
    pchild->reactEditPosAbsParent();
  }
}
void Element::notifyEditSizeAbs() {
  for (auto pchild : pchildren) {
    pchild->reactEditSizeAbsParent();
  }
}

void Element::reactEditPosAbsParent() {
  updatePosAbs();
}

void Element::reactEditSizeAbsParent() {
  updatePosAbs();
  updateSizeAbs();
}

void Element::setParent(Element* pparent) {
  this->pparent = pparent;
  updateDepth();
  reactEditPosAbsParent();
  reactEditSizeAbsParent();
}
const Element* Element::getParent() const {
  return pparent;
}

void Element::updateDepth() {
  if (pparent == nullptr)
    depthCache = 0;
  else
    depthCache = pparent->getDepth() + 1;

  for (auto pchild : pchildren) {
    pchild->updateDepth();
  }
}

unsigned int Element::getDepth() const {
  return depthCache;
}

std::string Element::posToStr(sf::Vector2f pos) const {
  std::stringstream stream;
  stream << "(";
  stream << std::setw(3) << pos.x;
  stream << ", ";
  stream << std::setw(3) << pos.y;
  stream << ")";
  return stream.str();
}

Element::operator string() const {
  std::stringstream stream;
  auto classname = typeid(*this).name();
  auto sep = string(depthCache * 4, ' ');

  stream << sep;
  stream << classname << endl;

  stream << sep;
  stream << "pos:  ";
  stream << "Abs";
  stream << posToStr(getPosAbs()) << ",";
  stream << "Rel";
  stream << (std::string)posRelative << endl;

  stream << sep;
  stream << "size: ";
  stream << "Abs" << posToStr(getSizeAbs()) << ",";
  stream << "Rel" << (std::string)sizeRelative << endl;
  return stream.str();
}

std::string Element::getTreeView() const {
  std::stringstream stream;
  string thisStr = *this;
  stream << thisStr;

  if (!pchildren.empty())
    stream << endl;

  size_t count = 0;
  for (auto pchild : pchildren) {
    stream << pchild->getTreeView();
    count++;
    if (count != pchildren.size())
      stream << endl;
  }
  if (depthCache == 0)
    stream << endl;

  return stream.str();
}

void Element::updatePosAbs() {
  sf::Vector2f parentPosAbs, parentSizeAbs;
  parentPosAbs = pparent ? pparent->getPosAbs() : sf::Vector2f{0, 0};
  parentSizeAbs = pparent ? pparent->getSizeAbs() : sf::Vector2f{0, 0};
  float x = computeCoord(posRelative.x, parentPosAbs.x, parentSizeAbs.x,
                         sizeAbsCache.x);
  float y = computeCoord(posRelative.y, parentPosAbs.y, parentSizeAbs.y,
                         sizeAbsCache.x);
  sf::Vector2f posAbsNew = {x, y};
  if (posAbsCache != posAbsNew) {
    posAbsCache = posAbsNew;
    notifyEditPosAbs();
  }
}

void Element::updateSizeAbs() {
  sf::Vector2f parentSizeAbs;
  parentSizeAbs = pparent ? pparent->getSizeAbs() : sf::Vector2f{0, 0};
  float x = computeLength(sizeRelative.x, parentSizeAbs.x);
  float y = computeLength(sizeRelative.y, parentSizeAbs.y);
  sf::Vector2f sizeAbsNew = {x, y};
  if (sizeAbsCache != sizeAbsNew) {
    sizeAbsCache = sizeAbsNew;
    notifyEditSizeAbs();
    updatePosAbs();
  }
}

float Element::computeCoord(Relatif rel,
                            float parentCoordAbs,
                            float parentLengthAbs,
                            float lengthAbs) {
  float f = parentCoordAbs;
  float pixel = rel.getPixel();
  switch (rel.getComputeMethod()) {
    case ComputeMethodType::pixel: {
      if (!pparent)
        f += pixel >= 0 ? pixel : 0;
      else
        f += pixel + (pixel < 0 ? parentLengthAbs : 0);
    }

    case ComputeMethodType::percent:
      if (!pparent)
        f += pixel >= 0 ? pixel : 0;
      else {
        auto percent = rel.getPercent();
        f += percent / 100 * parentLengthAbs;
        f += (percent < 0 ? parentLengthAbs : 0);
      }

    case ComputeMethodType::alignement: {
      if (!pparent)
        break;

      auto ali = rel.getAlignement();
      if (ali == "t" || ali == "l")
        break;
      else if (ali == "b" || ali == "r")
        f += parentLengthAbs - lengthAbs;
      else if (ali == "m")
        f += parentLengthAbs / 2 - lengthAbs / 2;
    }

    default:
      break;
  }
  return f;
}

float Element::computeLength(Relatif rel, float parentLengthAbs) {
  float f;
  switch (rel.getComputeMethod()) {
    case ComputeMethodType::pixel:
      return rel.getPixel();

    case ComputeMethodType::percent:
      if (pparent)
        return parentLengthAbs * rel.getPercent() / 100;
      else
        return 0;

    default:
      return 0;
  }

  return f;
}