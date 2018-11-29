#include "View.h"
#include <iostream>

using namespace render;
using namespace std;

View::View() {}

View::~View() {}

void View::updatePosAbs() {
  Element::updatePosAbs();
  sf::Vector2f size = getSizeAbs();
  sf::Vector2f sizeHalf = {size.x / 2, size.y / 2};
  sf::Vector2f center = getPosAbs() + sizeHalf;
  view.setCenter(center);
}

void View::updateSizeAbs() {
  Element::updateSizeAbs();
  view.setSize(getSizeAbs());
}