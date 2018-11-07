#include "ElementComposite.h"
#include <iostream>

using namespace render;
using namespace std;

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