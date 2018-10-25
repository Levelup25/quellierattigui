#include "TileSprite.h"

using namespace std;
using namespace sf;
using namespace render;

TileSprite::TileSprite(int l, int h, int nbRepeat) {
    this->l = l;
    this->h = h;
    this->nbRepeat = nbRepeat;
    sprites.resize(5);
    textures.resize(5);
    vector<string> numbers = {"00", "28", "55", "37", "93"};
    for (int i = 0; i < 5; i++) {
        textures[i].loadFromFile("res/tileset/isometric_pixel_flat_00" + numbers[i] + ".png", IntRect(0, 0, l / nbRepeat, h / nbRepeat));
        textures[i].setRepeated(true);
        IntRect rect(0, 0, l, h);
        sprites[i].setTextureRect(rect);
        sprites[i].setTexture(textures[i]);
    }
}

sf::Sprite TileSprite::getSprite(int element) {
    return sprites[element];
}