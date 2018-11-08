#include "Element.h"
#include <iostream>

using namespace render;
using namespace std;

Element::Element() {
  pos = {0, 0};
  size = {10, 10};
};

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto pchild : pchildren) {
    target.draw(*pchild);
  }
};

void Element::updateEvent(sf::Event event, sf::Vector2f posMouse){};

const sf::Vector2f Element::getPos() const {
  return pos;
}
void Element::setPos(const sf::Vector2f pos) {
  this->pos = pos;
  notifyPos();
}

const sf::Vector2f Element::getAbsPos() const {
  if (pparent) {
    float x, y;
    auto parentPos = pparent->getAbsPos();
    auto parentSize = pparent->getSize();
    if (pos.x > 0)
      x = parentPos.x + pos.x;
    else
      x = parentPos.x + parentSize.x + pos.x;

    if (pos.y > 0)
      y = parentPos.y + pos.y;
    else
      y = parentPos.y + parentSize.y + pos.y;

    return sf::Vector2f({x, y});
  }

  else
    return pos;
}

std::string Element::posToStr(sf::Vector2f pos) const {
  return "(" + to_string((int)pos.x) + ", " + to_string((int)pos.y) + ")";
}

const sf::Vector2f Element::getSize() const {
  return size;
}

void Element::setSize(const sf::Vector2f size) {
  this->size = size;
  notifySize();
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

void Element::notifyPos() {
  for (auto pchild : pchildren) {
    pchild->updatePosParent();
  }
}
void Element::notifySize() {
  for (auto pchild : pchildren) {
    pchild->updateSizeParent();
  }
}

void Element::updatePosParent() {}
void Element::updateSizeParent() {}

void Element::setParent(Element* pparent) {
  this->pparent = pparent;
  depth = pparent->depth + 1;
  updatePosParent();
  updateSizeParent();
}
const Element* Element::getParent() const {
  return pparent;
}

unsigned int Element::getDepth() const {
  return depth;
}

void Element::printTreeView() const {
  cout << string(depth, ' ') + string(depth, ' ');
  cout << posToStr(getAbsPos()) << " ";
  cout << posToStr(pos) << " ";
  cout << posToStr(size) << " ";
  cout << this << endl;
  for (auto pchild : pchildren) {
    pchild->printTreeView();
  }
  if (depth == 0)
    cout << endl;
}