#include "AbilitySprite.h"

using namespace std;
using namespace sf;
using namespace render;

AbilitySprite::AbilitySprite(int l, int h) {
    this->l = l;
    this->h = h;
}

sf::Sprite AbilitySprite::getSprite(int element, unsigned int lv) {
    vector<string> elem = {"dagger", "blizzard", "earth", "explosion", "wind"};
    texture.loadFromFile("res/attacks/" + elem[element] + "-" + to_string(lv) + ".png");
    sprite.setTexture(texture);
    sprite.setScale(Vector2f((float) l / 56, (float) h / 56));
    return sprite;
}

