#include "CharacterSprite.h"

using namespace std;
using namespace sf;
using namespace render;

CharacterSprite::CharacterSprite(int l, int h) {
    this->l = l;
    this->h = h;
    texture.loadFromFile("res/persos.png");
    sprite.setTexture(texture);
}

Sprite CharacterSprite::getSprite(int id, int direction, int animation) {
    vector<int> offsetI{0, 101, 210, 319, 434, 543, 654, 766, 876, 985, 1094, 1208};
    vector<int> offsetJ{0, 146, 290, 439};
    IntRect rect(animation * h + offsetI[id % 12], direction * h + offsetJ[id / 12], l, h);
    sprite.setTextureRect(rect);
    return sprite;
}

