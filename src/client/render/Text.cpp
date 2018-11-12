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

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(text);
  Element::draw(target, states);
};

void Text::updatePosAbs() {
  Element::updatePosAbs();
  text.setPosition(getPosAbs());
}

void Text::updateSizeAbs() {}