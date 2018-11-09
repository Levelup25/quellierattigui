#include "Element.h"
#include <iostream>

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
  notifyEditPosAbs();
}

const sf::Vector2f Element::getPosAbs() const {
  return posAbsCache;
}

std::string Element::posToStr(sf::Vector2f pos) const {
  return "(" + to_string((int)pos.x) + ", " + to_string((int)pos.y) + ")";
}

const sf::Vector2f Element::getSizeAbs() const {
  return sizeAbsCache;
}

void Element::setSizeRelative(const Relatif2 sizeRelative) {
  this->sizeRelative = sizeRelative;
  notifyEditSize();
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
void Element::notifyEditSize() {
  for (auto pchild : pchildren) {
    pchild->reactEditSizeParent();
  }
}

void Element::reactEditPosAbsParent() {
  updatePosAbs();
}

void Element::reactEditSizeParent() {}

void Element::setParent(Element* pparent) {
  this->pparent = pparent;
  updateDepth();
  reactEditPosAbsParent();
  reactEditSizeParent();
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

void Element::printTreeView() const {
  auto classname = typeid(*this).name();
  auto sep = string(depthCache, ' ');
  cout << sep;
  cout << posToStr(getPosAbs()) << " ";
  cout << (std::string)posRelative << " ";
  cout << posToStr(getSizeAbs()) << " ";
  cout << classname << endl;
  for (auto pchild : pchildren) {
    pchild->printTreeView();
  }
  if (depthCache == 0)
    cout << endl;
}

void Element::updatePosAbs() {
  float x, y;
  if (pparent) {
    auto parentPos = pparent->getPosAbs();
    auto parentSize = pparent->getSizeAbs();
    x = computeCoord(posRelative.x, parentPos.x, parentSize.x);
    y = computeCoord(posRelative.y, parentPos.y, parentSize.y);
  } else {
    x = posRelative.x.getOffset();
    y = posRelative.y.getOffset();
  }
  posAbsCache = {x, y};

  notifyEditPosAbs();
}

float Element::computeCoord(Relatif rel,
                            float parentCoordAbs,
                            float parentLengthAbs) {
  float f;
  if (rel.getComputeMethod() == 0) {
    float offsetx = rel.getOffset();
    if (offsetx >= 0)
      f = parentCoordAbs + offsetx;
    else
      f = parentCoordAbs + parentLengthAbs + offsetx;
  }

  return f;
}