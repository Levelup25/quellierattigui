#include "ContentSprite.h"

using namespace std;
using namespace sf;
using namespace render;

ContentSprite::ContentSprite(std::string filename, int l, int h) {
    this->l = l;
    this->h = h;
    texture.loadFromFile(filename);
    IntRect rect(0, 0, l, h);
    sprite.setTextureRect(rect);
    sprite.setTexture(texture);
}

sf::Sprite ContentSprite::getSprite(int element) {
    // 20 125 160 265
    // 10 145 165 300
    vector<int> offsetI{20, 160};
    vector<int> offsetJ{10, 165};
    if (element == 0) element = 4;
    IntRect rect(offsetI[(element - 1) % 2], offsetJ[(element - 1) / 2], l, h);
    sprite.setTextureRect(rect);
    return sprite;
}
