#include "ElementComposite.h"

using namespace render;

void ElementComposite::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const {
  for (auto pchild : children) {
    target.draw(*pchild);
  }
}