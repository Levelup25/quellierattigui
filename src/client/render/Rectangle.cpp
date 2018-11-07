#include "Rectangle.h"

using namespace render;

Rectangle::Rectangle() {}

const sf::RectangleShape& Rectangle::getRecshape() const {
  return recshape;
}

void Rectangle::setRecshape(const sf::RectangleShape& recshape) {
  this->recshape = recshape;
}

void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(recshape);
};

void Rectangle::updatePos() {
  recshape.setPosition(getAbsPos());
}