#include "Text.h"
#include <iostream>

using namespace render;
using namespace std;

sf::Font Text::roboto = sf::Font();
bool Text::robotoLoaded = false;

Text::Text() {
  if (!Text::robotoLoaded) {
    Text::roboto.loadFromFile(
        "res/font/roboto/"
        "Roboto-Regular.ttf");
    Text::robotoLoaded = true;
  }
  text.setFont(Text::roboto);
  text.setString("");
  text.setCharacterSize(20);
  text.setColor(sf::Color::Black);
}

Text::~Text() {}

Text::Text(const Text& obj) : Element::Element(obj) {
  *this = obj;
}

Element* Text::getCopy() const {
  return new Text(*this);
}

Text& Text::operator=(const Text& obj) {
  text = obj.text;
  return *this;
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(text);
  Element::draw(target, states);
};

void Text::updatePosAbs() {
  Element::updatePosAbs();
  text.setPosition(getPosAbs());
}

void Text::setString(std::string str) {
  text.setString(str);
  auto localBound = text.getLocalBounds();
  setSizeRelative({localBound.width, localBound.height});
}
