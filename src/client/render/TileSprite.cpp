#include "TileSprite.h"

using namespace std;
using namespace sf;
using namespace render;

TileSprite::TileSprite(std::string filename, int l, int h, int nbRepeat) {
    this->l = l;
    this->h = h;
    this->nbRepeat = nbRepeat;
    texture.loadFromFile(filename, IntRect(0, 0, l / nbRepeat, h / nbRepeat));
    texture.setRepeated(true);
    IntRect rect(0, 0, l, h);
    sprite.setTextureRect(rect);
    sprite.setTexture(texture);
}

sf::Sprite TileSprite::getSprite() {
    return sprite;
}