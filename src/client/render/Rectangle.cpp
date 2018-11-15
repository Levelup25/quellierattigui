#include "Rectangle.h"
#include <iostream>

using namespace render;
using namespace std;

Rectangle::Rectangle() {
  recshape.setOutlineThickness(1);
  recshape.setOutlineColor(sf::Color::Black);
  recshape.setFillColor(sf::Color::White);
}

Rectangle::Rectangle(const Rectangle& obj) : Element::Element(obj) {
  *this = obj;
}

Rectangle& Rectangle::operator=(const Rectangle& rec) {
  recshape = rec.recshape;
  return *this;
}

Rectangle::~Rectangle() {}

Element* Rectangle::getCopy() const {
  return new Rectangle(*this);
}

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
