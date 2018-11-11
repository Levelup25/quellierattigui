#include "Element.h"
#include <iomanip>
#include <iostream>
#include <sstream>

#define DEBUGING 0
#define IDDEBUG -1

using namespace render;
using namespace std;

int Element::created = 0;

Element::Element() {
  posRelative = {0, 0};
  sizeRelative = {10, 10};
  Element::created++;
  id = created;
  if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
    cout << ">>>create: " << id << endl;
  }
};

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto pchild : pchildren) {
    target.draw(*pchild);
  }
};

void Element::reactEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  notifyEvent(event, mousePosAbs);
};

void Element::notifyEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  for (auto pchild : pchildren) {
    pchild->reactEvent(event, mousePosAbs);
  }
}

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

const Relatif2 Element::getSizeRelative() const {
  return sizeRelative;
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
  if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
    cout << ">>>edit parent: " << pparent->getId() << "<-" << getId() << endl;
  }
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

namespace render {
std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vec) {
  os << "(";
  os << std::setw(3) << vec.x;
  os << ", ";
  os << std::setw(3) << vec.y;
  os << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Element& el) {
  auto classname = typeid(el).name();
  auto sep = string(el.getDepth() * 4, ' ');

  os << sep;
  os << classname;
  os << std::setw(3) << el.id;
  os << endl;

  os << sep;
  os << "pos:  ";
  os << "Abs";
  os << el.getPosAbs() << ",";
  os << "Rel";
  os << el.getPosRelative() << endl;

  os << sep;
  os << "size: ";
  os << "Abs" << el.getSizeAbs() << ",";
  os << "Rel" << el.getSizeRelative() << endl;
  return os;
}
}  // namespace render

std::string Element::getTreeView() const {
  std::stringstream stream;
  stream << *this;

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
                         sizeAbsCache.y);
  sf::Vector2f posAbsNew = {x, y};

  if (posAbsCache != posAbsNew) {
    posAbsCache = posAbsNew;
    if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
      cout << ">>>updatePosAbs " << id << endl;
      cout << (*this) << endl;
    }
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
    if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
      cout << ">>>updateSizeAbs " << id << endl;
      cout << (*this) << endl;
    }
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
      f += pixel;
      if (pparent) {
        float flimit = parentCoordAbs + parentLengthAbs - lengthAbs;
        if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG))
          cout << "flimit = " << flimit << " = " << parentCoordAbs << " + "
               << parentLengthAbs << "-" << lengthAbs << endl;

        f += (pixel < 0 ? parentLengthAbs - lengthAbs : 0);
        f = f > flimit ? flimit : f;
      }
      f = f < 0 ? 0 : f;
      break;
    }

    case ComputeMethodType::percent:
      if (!pparent)
        f += pixel >= 0 ? pixel : 0;
      else {
        auto percent = rel.getPercent();
        f += percent / 100 * parentLengthAbs;
        f += (percent < 0 ? parentLengthAbs - lengthAbs : 0);
      }
      break;

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
      break;
    }

    default:
      break;
  }
  return f;
}

float Element::computeLength(Relatif rel, float parentLengthAbs) {
  float f;
  switch (rel.getComputeMethod()) {
    case ComputeMethodType::pixel: {
      float pixel = rel.getPixel();
      if (!pparent)
        f = pixel >= 0 ? pixel : 0;
      else
        f = pixel + (pixel < 0 ? parentLengthAbs : 0);
      break;
    }

    case ComputeMethodType::percent: {
      if (pparent) {
        f = parentLengthAbs * rel.getPercent() / 100;
        f += (percent < 0 ? parentLengthAbs : 0);
      } else
        f = 0;
      break;
    }

    default:
      f = 0;
      break;
  }
  return f;
}

bool Element::isInside(sf::Vector2f posAbs) {
  return posAbsCache.x <= posAbs.x &&
         posAbs.x <= posAbsCache.x + sizeAbsCache.x &&
         posAbsCache.y <= posAbs.y &&
         posAbs.y <= posAbsCache.y + sizeAbsCache.y;
}

int Element::getId() {
  return id;
}