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

vector<Character*> Team::getCharacters(int i) {
    if (i < 0 || i >= (int)characters.size())return characters;
    else {
        vector<Character*> v;
        for (auto c = characters.begin(); c != characters.begin() + i; c++) v.push_back(*c);
        return v;
    }
}

void Team::swapCharacters(size_t i1, size_t i2) {
    iter_swap(characters.begin() + i1, characters.begin() + i2);
}

void Team::swapCharacters(Character* c1, Character* c2) {
    auto i1 = characters.begin(), i2 = characters.begin();
    for (auto c = characters.begin(); c != characters.end(); c++) {
        if (*c == c1) i1 = c;
        else if (*c == c2) i2 = c;
    }
    iter_swap(i1, i2);
}

string Team::getName() {
    return name;
}

Inventory* Team::getInventory() {
    return inventory;
}