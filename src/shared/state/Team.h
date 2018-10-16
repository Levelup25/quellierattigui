// Generated by dia2code
#ifndef STATE__TEAM__H
#define STATE__TEAM__H

#include <vector>
#include <memory>
#include <stdlib.h>

namespace state {
  class Inventory;
  class Character;
  class Weapon;
}

#include "Inventory.h"
#include "Character.h"

namespace state {

  /// class Team - 
  class Team {
    // Associations
    // Attributes
  protected:
    std::vector<std::unique_ptr<Character>> characters;
    std::unique_ptr<Inventory> inventory;
    // Operations
  public:
    Team ();
    void addCharacter (Character& character);
    void delCharacter (int i);
    void addWeapon (Weapon& weapon);
    void delWeapon (std::size_t i);
    Character& getMainCharacter ();
    Character& getCharacter (std::size_t i);
    void setMainCharacter (std::size_t i);
    Inventory& getInventory ();
    // Setters and Getters
    const std::vector<std::unique_ptr<Character>>& getCharacters() const;
    void setCharacters(const std::vector<std::unique_ptr<Character>>& characters);
    void setInventory(const std::unique_ptr<Inventory>& inventory);
  };

};

#endif
