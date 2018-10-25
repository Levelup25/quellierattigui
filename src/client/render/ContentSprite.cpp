#include "ContentSprite.h"

using namespace std;
using namespace sf;
using namespace render;

ContentSprite::ContentSprite(int l, int h) {
    this->l = l;
    this->h = h;
    sprites.resize(2);
    textures.resize(2);
    dimensions = {
        {105, 135},
        {85, 75}
    };
    offsets = {
        {
            {160, 20, 160, 20, 160},
            {10, 10, 10, 165, 165}
        },
        {
            {195, 195, 195, 195, 195},
            {15, 205, 15, 300, 105}
        }
    };
    vector<string> names = {"trees", "rocks"};
    for (int i = 0; i < 2; i++) {
        textures[i].loadFromFile("res/" + names[i] + ".png");
        IntRect rect(0, 0, dimensions[i][0], dimensions[i][1]);
        sprites[i].setTextureRect(rect);
        sprites[i].setTexture(textures[i]);
    }
}

sf::Sprite ContentSprite::getSprite(int content, int element) {
    content -= 1;
    IntRect rect(offsets[content][0][element], offsets[content][1][element], dimensions[content][0], dimensions[content][1]);
    sprites[content].setTextureRect(rect);
    sprites[content].setScale(Vector2f((float) l / dimensions[content][0], (float) h / dimensions[content][1]));
    return sprites[content];
}
