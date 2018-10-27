#include "World.h"
#include "AStar.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace state;

World::World(size_t i, size_t j) {
    // Initialize the grid with the good dimension and fill them with random cells
    I = i;
    J = j;

    srand(time(NULL));
    vector<ElementType> vector{wind, fire, earth, water};
    random_shuffle(vector.begin(), vector.end());
    vector.push_back(neutral);
    int r, r2;
    int p = 5, p2 = 10;
    grid.resize(I);
    for (size_t k = 0; k < I; k++) {
        grid[k].resize(J);
        for (size_t l = 0; l < J; l++) {
            Cell* cell = new Cell();
            r = rand() % 100;
            r2 = rand() % 100;
            cell->setContent(nothing);
            for (int m = 2; m < 4; m++) {
                if (r2 >= (m - 2) * p2 && r2 < (m - 1) * p2) {
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

void World::addCharacter() {
    Team* team = new Team();
    Character* character = new Character();
    team->addCharacter(character);
    teams.push_back(team);
}

void World::addCharacter(size_t iteam, int id, size_t i, size_t j, Direction direction) {
    Character* character = new Character(id);
    character->setDirection(direction);
    Team* team;
    while (teams.size() < iteam + 1) {
        team = new Team();
        teams.push_back(team);
    }
    this->addCharacter(character, teams[iteam], i, j);

}

void World::addCharacter(Character* character, Team* team, size_t i, size_t j) {
    size_t i2 = i, j2 = j;
    vector<Character*> characters = this->getCharacters();
    while (grid[i2][j2]->getContent() != nothing) {
        i2++;
        if (i2 >= I) {
            i2 = 0;
            j2++;
            if (j2 >= J) {
                j2 = 0;
            }
        }
        if (i == i2 && j == j2) return;
    }
    character->setI(i2);
    character->setJ(j2);
    team->addCharacter(character);
    grid[i2][j2]->setContent((ContentType) 1);
}

void World::moveCharacter(Character* character, size_t i, size_t j) {
    if (grid[i][j]->getContent() == nothing) {
        size_t i2 = character->getI(), j2 = character->getJ();
        grid[i2][j2]->setContent(nothing);
        size_t n = sqrt(I), m = sqrt(J);
        Character* maincharacter = (this->getMainCharacters())[0];
        int x = maincharacter->getI(), y = maincharacter->getJ();
        int xv = (x / n) * n, yv = (y / m) * m;
        AStar::Generator generator;
        generator.setWorldSize({n, m});
        for (int l = 0; l < m; l++) {
            for (int k = 0; k < n; k++) {
                if (grid[k + xv][l + yv]->getContent() != nothing) generator.addCollision({k, l});
            }
        }
        auto path = generator.findPath({i - xv, j - yv},
        {
            i2 - xv, j2 - yv
        });

        for (auto& coordinate : path) {
            cout << coordinate.x + xv << " " << coordinate.y + yv << endl;
        }
        cout << endl;

        character->setI(i);
        character->setJ(j);
        grid[i][j]->setContent((ContentType) 1);
        if (j > j2) character->setDirection(south);
        else if (i < i2) character->setDirection(west);
        else if (i > i2) character->setDirection(east);
        else if (j < j2) character->setDirection(north);
    }
}

void World::delCharacter(size_t i, size_t j) {
    for (auto t = teams.begin(); t != teams.end(); ++t) {
        vector<Character*> characters = (*t)->getCharacters();
        for (auto c = characters.begin(); c != characters.end(); ++c) {
            if ((*c)->getI() == i && (*c)->getJ() == j) {
                grid[i][j]->setContent(nothing);
                characters.erase(c);
                return;
            }
        }
    }
}

void World::delCharacter(Character* character) {
    for (auto t = teams.begin(); t != teams.end(); ++t) {
        vector<Character*> characters = (*t)->getCharacters();
        for (auto c = characters.begin(); c != characters.end(); ++c) {
            if (*c == character) {
                grid[character->getI()][ character->getJ()]->setContent(nothing);
                (*t)->delCharacter(*c);
                return;
            }
        }
    }
}

Character* World::getCharacter(size_t i, size_t j) {
    for (auto t = teams.begin(); t != teams.end(); ++t) {
        vector<Character*> characters = (*t)->getCharacters();
        for (auto c = characters.begin(); c != characters.end(); ++c) {
            if ((*c)->getI() == i && (*c)->getJ() == j) return *c;
        }
    }
    return nullptr;
}

vector<Character*> World::getMainCharacters() {
    vector<Character*> chars;
    for (auto t = teams.begin(); t != teams.end(); ++t) {
        chars.push_back((*t)->getMainCharacter());
    }
    return chars;
}

vector<Character*> World::getCharacters() {
    vector<Character*> chars;
    for (auto t = teams.begin(); t != teams.end(); ++t) {
        vector<Character*> characters = (*t)->getCharacters();
        for (auto c = characters.begin(); c != characters.end(); ++c) {
            chars.push_back(*c);
        }
    }
    return chars;
}

vector<Team*> World::getTeams() {
    return teams;
}

void World::addTeam() {
    Team* team = new Team();
    teams.push_back(team);
}

void World::addTeam(Team* team) {
    teams.push_back(team);
}