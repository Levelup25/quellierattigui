// Generated by dia2code
#ifndef STATE__ABILITY__H
#define STATE__ABILITY__H

#include <vector>

namespace state {
  class Cell;
}

#include "ElementType.h"
#include "ZoneType.h"

namespace state {

  /// class Ability - 
  class Ability {
    // Associations
    // Attributes
  private:
    int pa;
    int damage;
    ElementType element;
    int damageReduce;
    int cooldownInitial;
    int cooldownCurrent;
    ZoneType targetType;
    int targetMin;
    int targetMax;
    ZoneType effectType;
    int effectMin;
    int effectMax;
    // Operations
  public:
    Ability (int pa = 1, int damage = 1, ElementType element = neutral, int cooldown = 0, ZoneType targetType = circle, int targetMin = 1, int targetMax = 1, ZoneType effectType = circle, int effectMin = 0, int effectMax = 0);
    std::vector<Cell&> getPossibleTarget ();
    std::vector<Cell&> getEffectZone (Cell& target);
    void setCooldown (int initial);
    void setTarget (ZoneType targetZone, int min, int max);
    void setZone (ZoneType zone, int min, int max, int reduce);
    // Setters and Getters
  };

};

#endif
