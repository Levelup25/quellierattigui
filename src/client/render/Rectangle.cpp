#include "Rectangle.h"
#include <iostream>

using namespace render;
using namespace std;

Rectangle::Rectangle() {
  recshape.setOutlineThickness(1);
  recshape.setOutlineColor(sf::Color::Black);
  recshape.setFillColor(sf::Color::White);
}

void Rectangle::setPosRelative(const sf::Vector2f pos) {
  Element::setPosRelative(pos);
  recshape.setPosition(getAbsPos());
}

void Rectangle::setSize(const sf::Vector2f size) {
  Element::setSize(size);
  recshape.setSize(size);
}

// const sf::RectangleShape& Rectangle::getRecshape() const {
//   return recshape;
// }

// void Rectangle::setRecshape(const sf::RectangleShape& recshape) {
//   this->recshape = recshape;
// }

void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(recshape);
  Element::draw(target, states);
};

void Rectangle::updatePosParent() {
  recshape.setPosition(getAbsPos());
  Element::updatePosParent();
}