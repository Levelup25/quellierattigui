#include "Ability.h"
#include <fstream>
#include <iterator>
#include "Character.h"
#include "State.h"
#include "json/json.h"
#include "string.h"

using namespace std;
using namespace state;

Ability::Ability(int pa,
                 int damage,
                 ElementType element,
                 string name,
                 int lv,
                 int damageReduce,
                 int cooldown,
                 ZoneType targetType,
                 int targetMin,
                 int targetMax,
                 bool targetBlock,
                 ZoneType effectType,
                 int effectMin,
                 int effectMax,
                 bool effectBlock) {
  this->name = name;
  this->lv = lv;
  this->pa = pa;
  this->damage = damage;
  this->element = element;
  this->damageReduce = damageReduce;
  this->cooldownInitial = cooldown;
  this->targetType = targetType;
  this->targetMin = targetMin;
  this->targetMax = targetMax;
  this->targetBlock = targetBlock;
  this->effectType = effectType;
  this->effectMin = effectMin;
  this->effectMax = effectMax;
  this->effectBlock = effectBlock;
}

Ability::Ability(int id) {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  Json::Value ability;
  file.open("res/abilities.txt");
  reader.parse(file, root);
  ability = root[id];
  this->lv = ability.get("lv", 1).asInt();
  this->name = ability.get("name", "").asString();
  this->pa = ability.get("pa", 0).asInt();
  this->damage = ability.get("damage", 0).asInt();
  this->element = (ElementType)ability.get("element", 0).asInt();
  this->damageReduce = ability.get("damageReduce", 100).asInt();
  this->cooldownInitial = ability.get("cooldownInitial", 0).asInt();
  this->targetType = (ZoneType)ability.get("targetType", 0).asInt();
  this->targetMin = ability.get("targetMin", 0).asInt();
  this->targetMax = ability.get("targetMax", 0).asInt();
  this->targetBlock = ability.get("targetBlock", false).asBool();
  this->effectType = (ZoneType)ability.get("effectType", 0).asInt();
  this->effectMin = ability.get("effectMin", 0).asInt();
  this->effectMax = ability.get("effectMax", 0).asInt();
  this->effectBlock = ability.get("effectBlock", true).asBool();
  file.close();
}

Ability::Ability(string name) {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  Json::Value ability;
  file.open("res/abilities.txt");
  reader.parse(file, root);
  for (int id = 0; id < (int)root.size(); id++)
    if (name.compare(root[id].get("name", "").asString()) == 0) {
      ability = root[id];
      this->lv = ability.get("lv", 1).asInt();
      this->name = ability.get("name", "").asString();
      this->pa = ability.get("pa", 0).asInt();
      this->damage = ability.get("damage", 0).asInt();
      this->element = (ElementType)ability.get("element", 0).asInt();
      this->damageReduce = ability.get("damageReduce", 100).asInt();
      this->cooldownInitial = ability.get("cooldownInitial", 0).asInt();
      this->targetType = (ZoneType)ability.get("targetType", 0).asInt();
      this->targetMin = ability.get("targetMin", 0).asInt();
      this->targetMax = ability.get("targetMax", 0).asInt();
      this->targetBlock = ability.get("targetBlock", false).asBool();
      this->effectType = (ZoneType)ability.get("effectType", 0).asInt();
      this->effectMin = ability.get("effectMin", 0).asInt();
      this->effectMax = ability.get("effectMax", 0).asInt();
      this->effectBlock = ability.get("effectBlock", true).asBool();
    }
  file.close();
}

vector<vector<int>> Ability::getZone(vector<int> position,
                                     ZoneType zone,
                                     int min,
                                     int max) {
  vector<vector<int>> zones;
  int i = position[0], j = position[1];
  for (int k = min; k <= max; k++) {
    if (k == 0)
      zones.push_back({i, j});
    else {
      if (zone == circle) {
        for (int l = 0; l < k; l++) {
          zones.push_back({i - l, j - k + l});
          zones.push_back({i - k + l, j + l});
          zones.push_back({i + k - l, j - l});
          zones.push_back({i + l, j + k - l});
        }
      } else if (zone == line) {
        zones.push_back({i, j - k});
        zones.push_back({i - k, j});
        zones.push_back({i + k, j});
        zones.push_back({i, j + k});
      } else if (zone == diag) {
        zones.push_back({i - k, j - k});
        zones.push_back({i - k, j + k});
        zones.push_back({i + k, j - k});
        zones.push_back({i + k, j + k});
      } else if (zone == star) {
        zones.push_back({i, j - k});
        zones.push_back({i - k, j});
        zones.push_back({i + k, j});
        zones.push_back({i, j + k});
        zones.push_back({i - k, j - k});
        zones.push_back({i - k, j + k});
        zones.push_back({i + k, j - k});
        zones.push_back({i + k, j + k});
      }
    }
  }
  return zones;
}

vector<vector<int>> Ability::getTargetZone(vector<int> position) {
  return getZone(position, targetType, targetMin, targetMax);
}

vector<vector<int>> Ability::getEffectZone(vector<int> position) {
  return getZone(position, effectType, effectMin, effectMax);
}

void Ability::setCooldown(int initial) {
  cooldownInitial = initial;
}

void Ability::setTarget(ZoneType targetZone, int min, int max) {
  targetType = targetZone;
  targetMin = min;
  targetMax = max;
}

void Ability::setEffect(ZoneType effectZone, int min, int max, int reduce) {
  effectType = effectZone;
  effectMin = min;
  effectMax = max;
  damageReduce = reduce;
}

vector<int> Ability::getTarget() {
  return {(int)targetType, targetMin, targetMax, targetBlock};
}

vector<int> Ability::getEffect() {
  return {(int)effectType, effectMin, effectMax, effectBlock, damageReduce};
}

int Ability::getDamage(State* state, Character* att, Character* def) {
  if (state && element != neutral) {
    float mult = 1;
    int aelem = (ElementType)((int)element - 1);
    ElementType telem1 = state->getCell(att->getI(), att->getJ())->getElement();
    ElementType telem2 = state->getCell(def->getI(), def->getJ())->getElement();
    ElementType welem1 = att->getWeapon()->getElement();
    ElementType welem2 = def->getWeapon()->getElement();
    // vector<ElementType> elements = {water, earth, fire, wind};
    vector<ElementType> strengths = {fire, water, wind, earth};
    vector<ElementType> weaknesses = {wind, fire, earth, water};
    if (damage > 0) {
      if (strengths[aelem] == welem2)
        mult *= 2;
      else if (weaknesses[aelem] == welem2)
        mult *= 0.5;
      else if (element == welem2)
        mult *= 0.5;

      if (welem1 == telem1)
        mult *= 1.5;
      if (welem2 == telem2)
        mult /= 1.5;
    } else {
      if (strengths[aelem] == welem2)
        mult *= -1;
      else if (weaknesses[aelem] == welem2)
        mult *= 0.5;
      else if (element == welem2)
        mult *= 2;

      if (welem1 == telem1)
        mult *= 1.5;
      if (welem2 == telem2)
        mult *= 1.5;
    }
    return damage * mult;
  }
  return damage;
}

int Ability::getPa() {
  return pa;
}

string Ability::getName() {
  return name;
}

void Ability::setDamage(int dmg) {
  damage = dmg;
}

void Ability::setPa(int pa) {
  this->pa = pa;
}

void Ability::addLv() {
  lv += 1;
  pa += 1;
  if (damage > 0)
    damage += 1;
  else if (damage < 0)
    damage -= 1;
  targetMax += 1;
  effectMax += 1;
}

int Ability::getLv() {
  return lv;
}

void Ability::setElement(ElementType element) {
  this->element = element;
}

ElementType Ability::getElement() {
  return element;
}