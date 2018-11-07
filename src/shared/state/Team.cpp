#include "Team.h"

using namespace std;
using namespace state;

Team::Team(string name) {
    this->name = name;
    inventory = new Inventory();
}

void Team::addCharacter(Character* character) {
    characters.push_back(character);
}

void Team::delCharacter(Character* character) {
    for (auto c = characters.begin(); c != characters.end(); c++) {
        if (*c == character) {
            characters.erase(c);
            delete character;
            character = nullptr;
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

Character* Team::getCharacter(size_t i, size_t j) {
    for (auto c : characters) {
        if (c->getI() == i && c->getJ() == j) return c;
    }
    return nullptr;
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

bool Team::isAlive() {
    for (auto c : characters) {
        if (c->getPv() > 0) return true;
    }
    return false;
}