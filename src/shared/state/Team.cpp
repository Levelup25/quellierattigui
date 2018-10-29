#include "Team.h"

using namespace std;
using namespace state;

Team::Team(string name) {
    this->name = name;
}

void Team::addCharacter(Character* character) {
    characters.push_back(character);
}

void Team::delCharacter(Character* character) {
    for (auto c = characters.begin(); c != characters.end(); c++) {
        if (*c == character) {
            characters.erase(c);
            return;
        }
    }
}

Character* Team::getMainCharacter() {
    return characters[0];
}

Character* Team::getCharacter(size_t i) {
    return characters[i];
}

void Team::swapCharacters(size_t i1, size_t i2) {
    iter_swap(characters.begin() + i1, characters.begin() + i2);
}

const string& Team::getName() const {
    return name;
}

void Team::setName(const string& name) {
    this->name = name;
}

const vector<Character*>& Team::getCharacters() const {
    return characters;
}

void Team::setCharacters(const vector<Character*>& characters) {
    this->characters = characters;
}

const Inventory& Team::getInventory() const {
    return inventory;
}

void Team::setInventory(const Inventory& inventory) {
    this->inventory = inventory;
}