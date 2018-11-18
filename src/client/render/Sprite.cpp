#include "Sprite.h"
#include <iostream>

using namespace render;
using namespace std;

Sprite::Sprite() {}

Sprite::Sprite(const Sprite& sprite) : Element(sprite) {
  *this = sprite;
}

Sprite& Sprite::operator=(const Sprite& sprite) {
  this->sprite = sprite.sprite;
  return *this;
}

Element* Sprite::getCopy() const {
  return new Sprite(*this);
}

Sprite::~Sprite() {}

void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite);
  Element::draw(target, states);
};

void Sprite::updatePosAbs() {
  Element::updatePosAbs();
  sprite.setPosition(getPosAbs());
}

void Sprite::updateSizeAbs() {
  Element::updateSizeAbs();
  sf::IntRect rec;
  auto size = getSizeAbs();
  rec.width = size.x;
  rec.height = size.y;
  sprite.setTextureRect(rec);
}

void Sprite::updateSizeFromTextureRect() {
  sf::IntRect r = sprite.getTextureRect();
  sf::Vector2f scale = sprite.getScale();
  setSizeRelative({r.width * scale.x, r.height * scale.y});
}

std::ostream& operator<<(std::ostream& os, const Sprite& sprite) {
  os << (Element)sprite;
  auto sep = string(sprite.getDepth() * 4, ' ');
  os << sep;
  os << "&Texture: " << sprite.sprite.getTexture() << endl;
  return os;
}