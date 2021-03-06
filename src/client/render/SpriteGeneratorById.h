// Generated by dia2code
#ifndef RENDER__SPRITEGENERATORBYID__H
#define RENDER__SPRITEGENERATORBYID__H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

namespace render {

  /// class SpriteGeneratorById - 
  class SpriteGeneratorById {
    // Attributes
  private:
    std::map<int,  std::string> mapIdFile;
    std::map<int, sf::IntRect> mapIdRect;
    std::map<int, sf::Texture> mapIdTextureLoaded;
    // Operations
  public:
    SpriteGeneratorById (std::map<int,  std::string> mapIdFile, std::map<int,  sf::IntRect> mapIdRect);
    const sf::Texture* getTexture (int id);
    sf::Sprite getSpriteUnit (int id);
    sf::Sprite getSpriteRepeated (int id, sf::Vector2i nbRepeat);
    sf::Sprite getSprite (int id, sf::Vector2i size);
    sf::Sprite ResizeByScaling (sf::Sprite sprite, sf::Vector2i size);
    // Setters and Getters
  };

};

#endif
