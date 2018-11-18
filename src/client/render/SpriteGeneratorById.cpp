#include "SpriteGeneratorById.h"
#include <iostream>

using namespace render;
using namespace std;

SpriteGeneratorById::SpriteGeneratorById(std::map<int, std::string> mapIdFile,
                                         std::map<int, sf::IntRect> mapIdRect) {
  this->mapIdFile = mapIdFile;
  this->mapIdRect = mapIdRect;
}

const sf::Texture* SpriteGeneratorById::getTexture(int id) {
  if (mapIdTextureLoaded.find(id) != mapIdTextureLoaded.end()) {
    return &mapIdTextureLoaded[id];
  }

  sf::Texture t;
  if (mapIdRect.find(id) == mapIdRect.end()) {
    if (!t.loadFromFile(mapIdFile[id])) {
      cout << "error creating texture" << endl;
    }
  } else {
    if (!t.loadFromFile(mapIdFile[id], mapIdRect[id])) {
      cout << "error creating texture" << endl;
    }
  }

  t.setRepeated(false);
  mapIdTextureLoaded[id] = t;
  return &mapIdTextureLoaded[id];
}

sf::Sprite SpriteGeneratorById::getSpriteUnit(int id) {
  auto pt = getTexture(id);
  sf::Sprite s(*pt);
  return s;
}

sf::Sprite SpriteGeneratorById::getSpriteRepeated(int id, sf::Vector2i size) {
  auto s = getSpriteUnit(id);
  if (size.x < 1)
    size.x = 1;
  if (size.y < 1)
    size.y = 1;
  auto textRect = s.getTextureRect();
  textRect.width *= size.x;
  textRect.height *= size.y;
  s.setTextureRect(textRect);
  return s;
}

sf::Sprite SpriteGeneratorById::getSprite(int id, sf::Vector2i size) {
  return ResizeByScaling(getSpriteUnit(id), size);
}

sf::Sprite SpriteGeneratorById::ResizeByScaling(sf::Sprite sprite,
                                                sf::Vector2i size) {
  volatile auto textureRect = sprite.getTextureRect();
  sf::Vector2f sizeOriginal(
      {(float)textureRect.width, (float)textureRect.height});
  sf::Vector2f scale({size.x / sizeOriginal.x, size.y / sizeOriginal.y});
  sprite.setScale(scale);
  return sprite;
}