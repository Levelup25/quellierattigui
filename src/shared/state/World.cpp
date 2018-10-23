#include "World.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;
using namespace state;

World::World(size_t i, size_t j) {
    // Initialize the grid with the good dimension and fill them with empty cell
    I = i;
    J = j;
    srand(time(NULL));
    vector<ElementType> vector{wind, fire, earth, water};
    random_shuffle(vector.begin(), vector.end());
    vector.push_back(neutral);
    int r, r2;
    int p = 5, p2 = 5;
    grid.resize(i);
    for (size_t k = 0; k < i; k++) {
        grid[k].resize(j);
        for (size_t l = 0; l < j; l++) {
            Cell* cell = new Cell();
            r = rand() % 100;
            r2 = rand() % 100;
            cell->setContent(nothing);
            for (int m = 1; m < 4; m++) {
                if (r2 >= (m - 1) * p2 && r2 < m * p2) {
                    cell->setContent((ContentType) m);
                }
            }
            for (int m = 0; m < (int) vector.size(); m++) {
                if (r >= m * p && r < (m + 1) * p) {
                    cell->setElement((ElementType) m);
                }
            }
            if (r >= (int) vector.size() * p) {
                if (k >= I / 4 && k < 3 * I / 4 && l >= J / 4 && l < 3 * J / 4) {
                    cell->setElement(neutral);
                } else if (k < I / 2 && l < J / 2) {
                    cell->setElement(water);
                } else if (k < I / 2 && l >= J / 2) {
                    cell->setElement(earth);
                } else if (k >= I / 2 && l < J / 2) {
                    cell->setElement(fire);
                } else {
                    cell->setElement(wind);
                }
            }
            grid[k][l] = cell;
        }
    }
}

size_t World::getI() {
    return I;
}

size_t World::getJ() {
    return J;
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

void World::addCharacter(Character* character, std::size_t i, std::size_t j) {
    size_t i2 = i, j2 = j, ok = 0;
    while (!ok) {
        if (grid[i2][j2]->getContent() == nothing) {
            ok = 1;
            for (auto c = characters.begin(); c != characters.end(); ++c) {
                if ((*c)->getI() == i2 && (*c)->getJ() == j2) ok = 0;
            }
        }
        if (!ok) {
            i2++;
            if (i2 == I) {
                i2 = 0;
                j2++;
                if (j2 == J)return;
            }
        }
    }
    character->setI(i2);
    character->setJ(j2);
    characters.push_back(character);
}

void World::delCharacter(size_t i, size_t j) {
    for (auto c = characters.begin(); c != characters.end(); ++c) {
        if ((*c)->getI() == i && (*c)->getJ() == j) {
            characters.erase(c);
            return;
        }
    }
}

Character* World::getCharacter(size_t i, size_t j) {
    for (auto c = characters.begin(); c != characters.end(); ++c) {
        if ((*c)->getI() == i && (*c)->getJ() == j)
            return *c;
    }
    return nullptr;
}

vector<Character*> World::getCharacters() {
    return characters;
}