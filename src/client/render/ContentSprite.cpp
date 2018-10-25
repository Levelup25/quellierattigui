#include "ContentSprite.h"

using namespace std;
using namespace sf;
using namespace render;

ContentSprite::ContentSprite(std::string filename, int l, int h, vector<int> offsetI, vector<int> offsetJ) {
    this->l = l;
    this->h = h;
    this->offsetI = offsetI;
    this->offsetJ = offsetJ;
    texture.loadFromFile(filename);
    IntRect rect(0, 0, l, h);
    sprite.setTextureRect(rect);
    sprite.setTexture(texture);
}

sf::Sprite ContentSprite::getSprite(int element) {
    IntRect rect(offsetI[element], offsetJ[element], l, h);
    sprite.setTextureRect(rect);
    return sprite;
}
