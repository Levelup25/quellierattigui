#include "World.h"

using namespace std;
using namespace state;

World::World(size_t i, size_t j) {
    // Initialize the grid with the good dimension and fill them with empty cell
    I = i;
    J = j;
    grid.resize(i);
    for (size_t k = 0; k < i; k++) {
        grid[k].resize(j);
        for (size_t l = 0; l < j; l++) {
            Cell* cell = new Cell();
            grid[k][l] = cell;
        }
    }
}

vector<vector<Cell*>> World::getGrid() {
    return grid;
}

Cell* World::getCell(size_t i, size_t j) {
    return grid[i][j];
}

void World::setCell(size_t i, size_t j, Cell* cell) {
    grid[i][j] = cell;
}

void World::addCharacter(Character* character) {
    characters.push_back(character);
}

void World::delCharacter(size_t i, size_t j) {
    for (auto c = characters.begin(); c != characters.end(); ++c) {
        if ((*c)->getI() == i && (*c)->getJ() == j) characters.erase(c);
    }
}

Character* World::getCharacter(size_t i, size_t j) {
    for (auto c = characters.begin(); c != characters.end(); ++c) {
        if ((*c)->getI() == i && (*c)->getJ() == j) return *c;
    }
    return nullptr;
}

vector<Character*> World::getCharacters() {
    return characters;
}