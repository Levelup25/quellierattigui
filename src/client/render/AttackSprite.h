// Generated by dia2code
#ifndef RENDER__ATTACKSPRITE__H
#define RENDER__ATTACKSPRITE__H

#include <SFML/Graphics.hpp>

namespace render {

  /// class AttackSprite - 
  class AttackSprite {
    // Attributes
  private:
    sf::Texture texture;
    sf::Sprite sprite;
    int l;
    int h;
    // Operations
  public:
    AttackSprite (int l, int h);
    sf::Sprite getSprite (int element, unsigned int lv);
    // Setters and Getters
  };

};

#endif