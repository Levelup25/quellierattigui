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
    return characters[indexMainCharacter];
}

Character* Team::getCharacter(size_t i) {
    return characters[i];
}

void Team::setMainCharacter(size_t i) {
    indexMainCharacter = i;
}

const string& Team::getName() const {
    return name;
}

void Team::setName(const string& name) {
    this->name = name;
}

const size_t& Team::getIndexMainCharacter() const {
    return indexMainCharacter;
}

void Team::setIndexMainCharacter(const size_t& indexMainCharacter) {
    this->indexMainCharacter = indexMainCharacter;
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