#include "Rectangle.h"
#include <iostream>

using namespace render;
using namespace std;

Rectangle::Rectangle() {
  recshape.setOutlineThickness(1);
  recshape.setOutlineColor(sf::Color::Black);
  recshape.setFillColor(sf::Color::White);
}

Rectangle::~Rectangle() {}

void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(recshape);
  Element::draw(target, states);
};

void Rectangle::updatePosAbs() {
  Element::updatePosAbs();
  recshape.setPosition(getPosAbs());
}

void Rectangle::updateSizeAbs() {
  Element::updateSizeAbs();
  recshape.setSize(getSizeAbs());
}