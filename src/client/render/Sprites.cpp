#include "Sprites.h"
#include <iostream>

using namespace std;
using namespace sf;
using namespace render;

Sprites::Sprites(int nb) {
  this->nb = nb;
  tileTextures.resize(5);
  tileSprites.resize(5);
  contentTextures.resize(2);
  contentSprites.resize(2);
  characterTexture.loadFromFile("res/persos.png");
  characterSprite.setTexture(characterTexture);
  abilityTextures.resize(5);
  abilitySprites.resize(5);
  attackTexture.loadFromFile("res/fx.png");
  attackSprite.setTexture(attackTexture);

  vector<string> numbers = {"00", "28", "55", "37", "93"};
  for (int i = 0; i < (int)tileTextures.size(); i++) {
    tileTextures[i].loadFromFile(
        "res/tileset/isometric_pixel_flat_00" + numbers[i] + ".png",
        IntRect(0, 0, 34, 24));
    tileTextures[i].setRepeated(true);
    tileSprites[i].setTexture(tileTextures[i]);
  }

  vector<string> names = {"trees", "rocks"};
  for (int i = 0; i < (int)contentTextures.size(); i++) {
    contentTextures[i].loadFromFile("res/" + names[i] + ".png");
    contentSprites[i].setTexture(contentTextures[i]);
  }

  vector<string> elem = {"dagger", "blizzard", "earth", "explosion", "wind"};
  for (int i = 0; i < (int)abilityTextures.size(); i++) {
    abilityTextures[i].resize(3);
    abilitySprites[i].resize(3);
    for (int j = 0; j < (int)abilityTextures[i].size(); j++) {
      abilityTextures[i][j].loadFromFile("res/attacks/" + elem[i] + "-" +
                                         to_string(j + 1) + ".png");
      abilitySprites[i][j].setTexture(abilityTextures[i][j]);
    }
  }
}

sf::Sprite Sprites::getTileSprite(int l, int h, int element) {
  tileSprites[element].setTextureRect(IntRect(0, 0, l * nb, h * nb));
  //        tileSprites[element].setScale(Vector2f((float) l / 34,
  //                                               (float) h / 24));
  return tileSprites[element];
}

sf::Sprite Sprites::getContentSprite(int l, int h, int content, int element) {
  vector<vector<int>> dimensions{{105, 135}, {85, 75}};
  vector<vector<vector<int>>> offsets{
      {{160, 20, 160, 20, 160}, {10, 10, 10, 165, 165}},
      {{195, 195, 195, 195, 195}, {15, 205, 15, 300, 105}}};
  content -= 2;
  contentSprites[content].setTextureRect(
      IntRect(offsets[content][0][element], offsets[content][1][element],
              dimensions[content][0], dimensions[content][1]));
  contentSprites[content].setScale(Vector2f((float)l / dimensions[content][0],
                                            (float)h / dimensions[content][1]));
  return contentSprites[content];
}

sf::Sprite Sprites::getCharacterSprite(int l,
                                       int h,
                                       int id,
                                       int direction,
                                       int animation) {
  vector<int> offsetI{0,   101, 210, 319, 434,  543,
                      654, 766, 876, 985, 1094, 1208};
  vector<int> offsetJ{0, 146, 290, 439};
  characterSprite.setTextureRect(IntRect(animation * 32 + offsetI[id % 12],
                                         direction * 32 + offsetJ[id / 12], 32,
                                         32));
  characterSprite.setScale(Vector2f(nb, (float)h / 32));
  return characterSprite;
}

sf::Sprite Sprites::getAbilitySprite(int l,
                                     int h,
                                     int element,
                                     unsigned int lv) {
  lv--;
  abilitySprites[element][lv].setScale(Vector2f((float)l / 56, (float)h / 56));
  return abilitySprites[element][lv];
}

sf::Sprite Sprites::getAttackSprite(int l,
                                    int h,
                                    int direction,
                                    int element,
                                    int lv) {
  lv--;
  vector<Color> colors{sf::Color(255, 255, 255), Color::Blue, Color::Green,
                       Color::Red, Color::Yellow};
  vector<int> size{23, 32, 32};
  vector<int> offsetI{24, 281, 281};
  vector<int> offsetJ{288, 231, 197};
  attackSprite.setTextureRect(IntRect(offsetI[lv] + direction * size[lv],
                                      offsetJ[lv], size[lv], size[lv]));
  attackSprite.setScale(Vector2f((float)l / 32, (float)h / 32));
  attackSprite.setColor(colors[element]);
  return attackSprite;
}
