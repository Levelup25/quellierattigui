// Generated by dia2code
#ifndef RENDER__SPRITES__H
#define RENDER__SPRITES__H

#include <vector>
#include <SFML/Graphics.hpp>

namespace render {

  /// class Sprites - 
  class Sprites {
    // Attributes
  private:
    int nb;
    std::vector<sf::Texture> tileTextures;
    std::vector<sf::Sprite> tileSprites;
    std::vector<sf::Texture> contentTextures;
    std::vector<sf::Sprite> contentSprites;
    sf::Texture characterTexture;
    sf::Sprite characterSprite;
    std::vector<std::vector<sf::Texture>> abilityTextures;
    std::vector<std::vector<sf::Sprite>> abilitySprites;
    sf::Texture attackTexture;
    sf::Sprite attackSprite;
    sf::Texture weaponTexture;
    sf::Sprite weaponSprite;
    // Operations
  public:
    Sprites (int nb = 2);
    sf::Sprite getTileSprite (int l, int h, int element);
    sf::Sprite getContentSprite (int l, int h, int content, int element = 0);
    sf::Sprite getCharacterSprite (int l, int h, int id, int direction = 0, int animation = 1);
    sf::Sprite getAbilitySprite (int l, int h, int element, int lv);
    sf::Sprite getAttackSprite (int l, int h, int direction, int element, int lv);
    sf::Sprite getWeaponSprite (int l, int h, int id);
    // Setters and Getters
  };

};

#endif
