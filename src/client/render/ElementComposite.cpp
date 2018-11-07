#include "ElementComposite.h"
#include <iostream>
#include "Rectangle.h"

using namespace render;
using namespace std;

ElementComposite::ElementComposite() {
  cout << "ec init" << endl;
  posParent = sf::Vector2f(0, 0);
  size = sf::Vector2f(0, 0);

  pborder = new Rectangle();

  auto borderRecshape = pborder->getRecshape();
  borderRecshape.setSize(size);
  borderRecshape.setOutlineThickness(1);
  borderRecshape.setOutlineColor(sf::Color::Black);
  borderRecshape.setFillColor(sf::Color::White);
  borderRecshape.setPosition(posParent);
  pborder->setRecshape(borderRecshape);

  add(pborder);
}

void ElementComposite::setSize(const sf::Vector2f& size) {
  this->size = size;
  auto borderRecshape = pborder->getRecshape();
  borderRecshape.setSize(size);
  pborder->setRecshape(borderRecshape);
}

void ElementComposite::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
  for (auto pchild : children) {
    target.draw(*pchild);
  }
}

const std::vector<Element*>& ElementComposite::getChildren() const {
  return children;
}
void ElementComposite::setChildren(const std::vector<Element*>& children) {
  this->children = children;
}

void ElementComposite::add(Element* pchild) {
  children.push_back(pchild);
}

void ElementComposite::remove(Element* pchild) {
  for (auto it = children.begin(); it != children.end(); it++) {
    auto p = *it;
    if (p == pchild) {
      children.erase(it);
      return;
    }
  }
}