#include "AttackSprite.h"

using namespace std;
using namespace sf;
using namespace render;

AttackSprite::AttackSprite(int l, int h) {
    this->l = l;
    this->h = h;
    texture.loadFromFile("res/fx.png");
    sprite.setTexture(texture);
    sprite.setScale(Vector2f((float) l / 32, (float) h / 32));
}

sf::Sprite AttackSprite::getSprite(int direction, int element, int lv) {
    lv -= 1;
    vector<Color> colors{sf::Color(255, 255, 255), Color::Blue, Color::Green, Color::Red, Color::Yellow};
    vector<int> size{23, 32, 32};
    vector<int> offsetI{24, 281, 281};
    vector<int> offsetJ{288, 231, 197};
    IntRect rect(offsetI[lv] + direction * size[lv], offsetJ[lv], size[lv], size[lv]);
    sprite.setTextureRect(rect);
    sprite.setColor(colors[element]);
    //sprite.setScale(Vector2f((float) l / size[lv], (float) h / size[lv]));
    return sprite;
}