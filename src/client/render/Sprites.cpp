//#include "Sprites.h"
//
//using namespace std;
//using namespace sf;
//using namespace render;
//
//Sprites::Sprites() {
//  this->l = l;
//  this->h = h;
//  this->nbRepeat = nbRepeat;
//  sprites.resize(5);
//  textures.resize(5);
//  vector<string> numbers = {"00", "28", "55", "37", "93"};
//  for (int i = 0; i < 5; i++) {
//    textures[i].loadFromFile(
//        "res/tileset/isometric_pixel_flat_00" + numbers[i] + ".png",
//        IntRect(0, 0, l / nbRepeat, h / nbRepeat));
//    textures[i].setRepeated(true);
//    IntRect rect(0, 0, l, h);
//    sprites[i].setTextureRect(rect);
//    sprites[i].setTexture(textures[i]);
//  }
//}
//
//sf::Sprite getTileSprite(int l, int h, int element) {}
//
//sf::Sprite getContentSprite(int l, int h, int content, int element = 0) {}
//
//sf::Sprite getCharacterSprite(int l,
//                              int h,
//                              int id,
//                              int direction = 0,
//                              int animation = 1) {}
//
//sf::Sprite getAbilitySprite(int l, int h, int element, unsigned int lv) {}
//
//sf::Sprite getAttackSprite(int l, int h, int direction, int element, int lv) {}
