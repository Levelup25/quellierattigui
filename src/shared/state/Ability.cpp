#include "Ability.h"
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;
using namespace state;

Ability::Ability(std::string name,
                 int lv,
                 int pa,
                 int damage,
                 ElementType element,
                 int damageReduce,
                 int cooldown,
                 ZoneType targetType,
                 int targetMin,
                 int targetMax,
                 ZoneType effectType,
                 int effectMin,
                 int effectMax) {
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
  this->effectType = effectType;
  this->effectMin = effectMin;
  this->effectMax = effectMax;
}

Ability::Ability(int id) {
  string line;
  ifstream file("res/abilities.txt");

  if (file.is_open()) {
    while (!file.eof()) {
      for (int i = 0; i <= id; i++)
        getline(file, line);
      istringstream iss(line);
      vector<std::string> results(istream_iterator<string>{iss},
                                  istream_iterator<string>());
      this->name = results[1];
      this->pa = atoi(results[2].c_str());
      this->damage = atoi(results[3].c_str());
      this->element = (ElementType)atoi(results[4].c_str());
      this->damageReduce = atoi(results[5].c_str());
      this->cooldownInitial = atoi(results[6].c_str());
      this->targetType = (ZoneType)atoi(results[7].c_str());
      this->targetMin = atoi(results[8].c_str());
      this->targetMax = atoi(results[9].c_str());
      this->effectType = (ZoneType)atoi(results[10].c_str());
      this->effectMin = atoi(results[11].c_str());
      this->effectMax = atoi(results[12].c_str());
      break;
    }
    file.close();
  }
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
      } else if (zone == line || zone == star) {
        zones.push_back({i, j - k});
        zones.push_back({i - k, j});
        zones.push_back({i + k, j});
        zones.push_back({i, j + k});

      } else if (zone == diag || zone == star) {
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
  return {(int)targetType, targetMin, targetMax};
}

vector<int> Ability::getEffect() {
  return {(int)effectType, effectMin, effectMax, damageReduce};
}

int Ability::getDamage() {
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
  damage += 1;
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