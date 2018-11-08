#include "Element.h"
#include <iostream>

using namespace render;
using namespace std;

Element::Element() {
  posRelative = {0, 0};
  size = {10, 10};
};

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto pchild : pchildren) {
    target.draw(*pchild);
  }
};

void Element::reactEvent(sf::Event event, sf::Vector2f posMouse){};

const sf::Vector2f Element::getPosRelative() const {
  return posRelative;
}
void Element::setPosRelative(const sf::Vector2f pos) {
  this->posRelative = pos;
  updatePosAbs();
  notifyEditPosAbs();
}

const sf::Vector2f Element::getPosAbs() const {
  return posAbs;
}

std::string Element::posToStr(sf::Vector2f pos) const {
  return "(" + to_string((int)pos.x) + ", " + to_string((int)pos.y) + ")";
}

const sf::Vector2f Element::getSize() const {
  return size;
}

void Element::setSize(const sf::Vector2f size) {
  this->size = size;
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
  updatePosAbs();
  reactEditPosAbsParent();
  reactEditSizeParent();
}
const Element* Element::getParent() const {
  return pparent;
}

void Element::updateDepth() {
  if (pparent == nullptr)
    depth = 0;
  else
    depth = pparent->getDepth() + 1;

  for (auto pchild : pchildren) {
    pchild->updateDepth();
  }
}

unsigned int Element::getDepth() const {
  return depth;
}

void Element::printTreeView() const {
  auto classname = typeid(*this).name();
  auto sep = string(depth, ' ');
  cout << sep;
  cout << posToStr(getPosAbs()) << " ";
  cout << posToStr(posRelative) << " ";
  cout << posToStr(size) << " ";
  cout << classname << endl;
  for (auto pchild : pchildren) {
    pchild->printTreeView();
  }
  if (depth == 0)
    cout << endl;
}

void Element::updatePosAbs() {
  if (pparent) {
    float x, y;
    auto parentPos = pparent->getPosAbs();
    auto parentSize = pparent->getSize();
    if (posRelative.x >= 0)
      x = parentPos.x + posRelative.x;
    else
      x = parentPos.x + parentSize.x + posRelative.x;

    if (posRelative.y >= 0)
      y = parentPos.y + posRelative.y;
    else
      y = parentPos.y + parentSize.y + posRelative.y;

    posAbs = {x, y};
  }

  else
    posAbs = posRelative;

  notifyEditPosAbs();
}