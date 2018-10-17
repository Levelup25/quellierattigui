// Generated by dia2code
#ifndef STATE__CHARACTER__H
#define STATE__CHARACTER__H

#include <stdlib.h>
#include <string>
#include <memory>

namespace state {
  class Weapon;
}

#include "CharacterType.h"
#include "Weapon.h"

namespace state {

  /// class Character - 
  class Character {
    // Associations
    // Attributes
  private:
    std::size_t i;
    std::size_t j;
    std::string nom;
    CharacterType type;
    int pvCurrent;
    int pvMax;
    int pmCurrent;
    int pmMax;
    int paCurrent;
    int paMax;
    std::unique_ptr<Weapon> weapon;
    // Operations
  public:
    Character (std::size_t i, std::size_t j, std::string nom = "", CharacterType type = human, int pv = 3, int pa = 3, int pm = 3);
    void setWeapon (Weapon& weapon);
    void removeWeapon ();
    // Setters and Getters
  };

};

#endif