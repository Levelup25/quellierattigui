#include "Weapon.h"
#include <fstream>
#include <iterator>
#include <sstream>

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
  string line;
  ifstream file("res/weapons.txt");

  if (file.is_open()) {
    while (!file.eof()) {
      for (int i = 0; i <= id; i++)
        getline(file, line);
      istringstream iss(line);
      vector<std::string> results(istream_iterator<string>{iss},
                                  istream_iterator<string>());
      this->name = results[1];
      this->element = (ElementType)atoi(results[2].c_str());
      for (unsigned int j = 3; j < results.size(); j++)
        abilities.push_back(new Ability(atoi(results[j].c_str())));
      break;
    }
    file.close();
  }
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