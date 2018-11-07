#include "Element.h"

using namespace render;
using namespace std;

Element::Element() {
  posParent = {0, 0};
  pos = {0, 0};
};

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {};

void Element::receiveEvent(sf::Event event, sf::Vector2f posMouse){};

const sf::Vector2f& Element::getPosParent() const {
  return posParent;
}
void Element::setPosParent(const sf::Vector2f& posParent) {
  this->posParent = posParent;
}
const sf::Vector2f& Element::getPos() const {
  return pos;
}
void Element::setPos(const sf::Vector2f& pos) {
  this->pos = pos;
  updatePos();
}

void Element::updatePos() {}

sf::Vector2f Element::getAbsPos() const {
  return posParent + pos;
}

std::string Element::posToStr(sf::Vector2f pos) const {
  return "(" + to_string(pos.x) + ", " + to_string(pos.y) + ")";
}