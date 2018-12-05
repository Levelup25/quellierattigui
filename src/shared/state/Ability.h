// Generated by dia2code
#ifndef STATE__ABILITY__H
#define STATE__ABILITY__H

#include <string>
#include <vector>

#include "ElementType.h"
#include "ZoneType.h"

namespace state {

  /// class Ability - 
  class Ability {
    // Associations
    // Attributes
  private:
    std::string name;
    int lv     = 1;
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
    Ability (std::string name = "", int lv = 1, int pa = 1, int damage = 1, ElementType element = neutral, int damageReduce = 100, int cooldown = 0, ZoneType targetType = circle, int targetMin = 1, int targetMax = 1, ZoneType effectType = circle, int effectMin = 0, int effectMax = 0);
    Ability (int id);
    std::vector<std::vector<int>> getTargetZone (std::vector<int> position);
    std::vector<std::vector<int>> getEffectZone (std::vector<int> position);
    void setCooldown (int initial);
    void setTarget (ZoneType targetZone, int min, int max);
    void setEffect (ZoneType effectZone, int min, int max, int reduce = 100);
    std::vector<int> getTarget ();
    std::vector<int> getEffect ();
    int getDamage ();
    int getPa ();
    std::string getName ();
    void setDamage (int dmg);
    void setPa (int pa);
    void addLv ();
    int getLv ();
    void setElement (ElementType element);
    ElementType getElement ();
  private:
    std::vector<std::vector<int>> getZone (std::vector<int> position, ZoneType zone, int min, int max);
    // Setters and Getters
  };

};

#endif
