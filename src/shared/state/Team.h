// Generated by dia2code
#ifndef STATE__TEAM__H
#define STATE__TEAM__H

#include <string>
#include <vector>
#include <stdlib.h>

namespace state {
  class Character;
  class Inventory;
}

#include "Character.h"
#include "Inventory.h"

namespace state {

  /// class Team - 
  class Team {
    // Associations
    // Attributes
  private:
    std::string name;
    std::vector<Character*> characters;
    Inventory* inventory;
    // Operations
  public:
    Team (std::string name = "");
    void addCharacter (Character* character);
    void delCharacter (Character* character);
    Character* getMainCharacter ();
    Character* getCharacter (std::size_t i);
    Character* getCharacter (std::size_t i, std::size_t j);
    std::vector<Character*> getCharacters (int i = -1);
    void swapCharacters (std::size_t i1, std::size_t i2);
    void swapCharacters (Character* c1, Character* c2);
    std::string getName ();
    Inventory* getInventory ();
    // Setters and Getters
  };

};

#endif
