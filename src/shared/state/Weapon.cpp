#include "Weapon.h"
#include <fstream>
#include <iterator>
#include "json/json.h"
#include "string.h"

using namespace std;
using namespace state;

Weapon::Weapon(ElementType element,
               std::string name,
               std::vector<Ability*> abilities) {
  this->element = element;
  this->name = name;
  this->abilities = abilities;
}

Weapon::Weapon(int id) {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  Json::Value weapon;
  file.open("res/weapons.txt");
  reader.parse(file, root);
  weapon = root[id];
  this->id = weapon.get("id", 0).asInt();
  this->name = weapon.get("name", "").asString();
  this->element = (ElementType)weapon.get("element", 0).asInt();
  for (int i = 0; i < (int)weapon.get("abilities", 0).size(); i++)
    abilities.push_back(new Ability(weapon.get("abilities", 0)[i].asString()));
  file.close();
}

Weapon::Weapon(string name) {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  Json::Value weapon;
  file.open("res/weapons.txt");
  reader.parse(file, root);
  for (int id = 0; id < (int)root.size(); id++)
    if (name.compare(root[id].get("name", "").asString()) == 0) {
      weapon = root[id];
      this->id = weapon.get("id", 0).asInt();
      this->name = weapon.get("name", "").asString();
      this->element = (ElementType)weapon.get("element", 0).asInt();
      for (int i = 0; i < (int)weapon.get("abilities", 0).size(); i++)
        abilities.push_back(
            new Ability(weapon.get("abilities", 0)[i].asString()));
    }
  file.close();
}

void Weapon::addAbility(Ability* ability) {
  abilities.push_back(ability);
}

void Weapon::delAbility(Ability* ability) {
  for (auto ab = abilities.begin(); ab != abilities.end(); ab++) {
    if (*ab == ability) {
      abilities.erase(ab);
      delete ability;
      ability = nullptr;
    }
  }
}

Ability* Weapon::getAbility(size_t i) {
  return abilities[i];
}

vector<Ability*> Weapon::getAbilities() {
  return abilities;
}

string Weapon::getName() {
  return name;
}

ElementType Weapon::getElement() {
  return element;
}

int Weapon::getId() {
  return id;
}

void Weapon::setId(int id) {
  this->id = id;
}