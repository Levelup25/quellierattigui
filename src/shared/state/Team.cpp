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

vector<Character*> Team::getCharacters() {
    return characters;
}

void Team::swapCharacters(size_t i1, size_t i2) {
    iter_swap(characters.begin() + i1, characters.begin() + i2);
}

string Team::getName() {
    return name;
}

Inventory* Team::getInventory() {
    return inventory;
}