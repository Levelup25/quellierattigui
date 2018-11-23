#include "State.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace state;

State::State(size_t i, size_t j) {
  // Initialize the grid with the good dimension and fill them with random cells
  I = i;
  J = j;

  vector<ElementType> vector{wind, fire, earth, water};
  random_shuffle(vector.begin(), vector.end());
  vector.push_back(neutral);
  int r, r2;
  int p = 5, p2 = 5;
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
          cell->setContent((ContentType)m);
        }
      }
      for (int m = 0; m < (int)vector.size(); m++) {
        if (r >= m * p && r < (m + 1) * p) {
          cell->setElement((ElementType)m);
        }
      }
      if (r >= (int)vector.size() * p) {
        if (k >= I / 4 && k < 3 * I / 4 && l >= J / 4 && l < 3 * J / 4) {
          cell->setElement(vector[4]);
        } else if (k < I / 2 && l < J / 2) {
          cell->setElement(vector[3]);
        } else if (k < I / 2 && l >= J / 2) {
          cell->setElement(vector[2]);
        } else if (k >= I / 2 && l < J / 2) {
          cell->setElement(vector[1]);
        } else {
          cell->setElement(vector[0]);
        }
      }
      grid[k][l] = cell;
    }
  }
}

void State::resetContents() {
  int i0 = ((int)this->getMainCharacters()[0]->getI() / n) * n,
      j0 = ((int)this->getMainCharacters()[0]->getJ() / m) * m;
  for (int i = i0; i < i0 + (int)n; i++)
    for (int j = j0; j < j0 + (int)m; j++)
      if (grid[i][j]->getContent() == perso)
        grid[i][j]->setContent(nothing);
  if (!this->isFighting())
    for (auto c : this->getMainCharacters())
      grid[c->getI()][c->getJ()]->setContent(perso);
  else
    for (auto c : this->getFight()->getFightingCharacters())
      grid[c->getI()][c->getJ()]->setContent(perso);
}

size_t State::getI() {
  return I;
}

size_t State::getJ() {
  return J;
}

size_t State::getN() {
  return n;
}

size_t State::getM() {
  return m;
}

vector<vector<Cell*>> State::getGrid() {
  return grid;
}

Cell* State::getCell(size_t i, size_t j) {
  return grid[i][j];
}

void State::setCell(size_t i, size_t j, Cell* cell) {
  grid[i][j] = cell;
}

void State::addCharacter() {
  Team* team = new Team();
  Character* character = new Character();
  team->addCharacter(character);
  teams.push_back(team);
}

void State::addCharacter(size_t iteam,
                         int id,
                         Direction direction,
                         size_t i,
                         size_t j) {
  Character* character = new Character(id);
  character->setDirection(direction);
  Team* team;
  while (teams.size() < iteam + 1) {
    team = new Team();
    teams.push_back(team);
  }
  this->addCharacter(character, teams[iteam], i, j);
}

void State::addCharacter(Character* character, Team* team, size_t i, size_t j) {
  size_t i2 = i, j2 = j;
  while (grid[i2][j2]->getContent() != nothing) {
    i2++;
    if (i2 % n == 0) {
      i2 -= n;
      j2++;
      if (j2 % m == 0) {
        j2 -= m;
        i2 += n;
      }
    }
    if (i2 >= I) {
      i2 = 0;
      j2++;
      if (j2 >= J) {
        j2 = 0;
      }
    }
    if (i == i2 && j == j2)
      return;
  }
  character->setI(i2);
  character->setJ(j2);
  team->addCharacter(character);
  if (character == team->getMainCharacter())
    grid[i2][j2]->setContent((ContentType)1);
}

void State::moveCharacter(Character* character, float i, float j) {
  float i0 = character->getI(), j0 = character->getJ();
  grid[(int)i0][(int)j0]->setContent(nothing);
  character->setI(i);
  character->setJ(j);
  grid[(int)i][(int)j]->setContent(perso);
}

void State::delCharacter(size_t i, size_t j) {
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    vector<Character*> characters = (*t)->getCharacters();
    for (auto c = characters.begin(); c != characters.end(); ++c) {
      if ((*c)->getI() == i && (*c)->getJ() == j) {
        grid[i][j]->setContent(nothing);
        (*t)->delCharacter(*c);
        return;
      }
    }
  }
}

void State::delCharacter(Character* character) {
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    vector<Character*> characters = (*t)->getCharacters();
    for (auto c = characters.begin(); c != characters.end(); ++c) {
      if (*c == character) {
        grid[character->getI()][character->getJ()]->setContent(nothing);
        (*t)->delCharacter(*c);
        return;
      }
    }
  }
}

Character* State::getCharacter(size_t i, size_t j) {
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    vector<Character*> characters = (*t)->getCharacters();
    for (auto c = characters.begin(); c != characters.end(); ++c) {
      if ((*c)->getI() == i && (*c)->getJ() == j)
        return *c;
    }
  }
  return nullptr;
}

vector<Character*> State::getMainCharacters() {
  vector<Character*> chars;
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    chars.push_back((*t)->getMainCharacter());
  }
  return chars;
}

vector<Character*> State::getCharacters() {
  vector<Character*> chars;
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    vector<Character*> characters = (*t)->getCharacters();
    for (auto c = characters.begin(); c != characters.end(); ++c) {
      chars.push_back(*c);
    }
  }
  return chars;
}

vector<Team*> State::getTeams() {
  return teams;
}

Team* State::getTeam(Character* character) {
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    vector<Character*> characters = (*t)->getCharacters();
    for (auto c = characters.begin(); c != characters.end(); ++c) {
      if ((*c) == character)
        return *t;
    }
  }
  return nullptr;
}

void State::addTeam() {
  Team* team = new Team();
  teams.push_back(team);
}

void State::addTeam(Team* team) {
  teams.push_back(team);
}

Team* State::getMainTeam() {
  return teams[0];
}

void State::delTeam(Team* team) {
  for (auto t = teams.begin(); t != teams.end(); ++t) {
    if ((*t) == team) {
      teams.erase(t);
      delete team;
      team = nullptr;
      return;
    }
  }
}

Character* State::getMainCharacter() {
  return teams[0]->getMainCharacter();
}

int State::getEpoch() const {
  return epoch;
}

void State::setEpoch(int epoch) {
  this->epoch = epoch;
}

int State::getEpochRate() const {
  return epochRate;
}

void State::setEpochRate(int epochRate) {
  this->epochRate = epochRate;
}

Fight* State::getFight() {
  return fight;
}

void State::setFight(Fight* fight) {
  this->fight = fight;
}

bool State::isFighting() {
  return fight != nullptr;
}

void State::deploy(int nb) {
  for (auto c : fight->getTeam(0)->getCharacters(nb)) {
    c->resetPm();
    c->resetPa();
    c->resetPv();
  }

  Team* team1 = fight->getTeam(0);
  Team* team2 = fight->getTeam(1);
  int i0 = team2->getMainCharacter()->getI(),
      j0 = team2->getMainCharacter()->getJ();
  int xv = (i0 / n) * n, yv = (j0 / m) * m;

  if (this->isFighting()) {
    int i, j, nb = fight->getNb();
    for (auto mainchars : team1->getCharacters(nb)) {
      do {
        i = xv + n / 6 + rand() % (2 * n / 3);
        j = yv + 2 * m / 3 + rand() % (m / 4);
      } while (grid[i][j]->getContent() != nothing);
      this->moveCharacter(mainchars, i, j);
      mainchars->setDirection(north);
    }
    for (auto oppchars : team2->getCharacters(nb)) {
      do {
        i = xv + n / 6 + rand() % (2 * n / 3);
        j = yv + m / 12 + rand() % (m / 4);
      } while (grid[i][j]->getContent() != nothing);
      this->moveCharacter(oppchars, i, j);
      oppchars->setDirection(south);
    }
  }
  this->resetContents();
  // 2/12   8/12   2/12   =>  1/6     2/3     1/6

  // 1/12 => 1/12
  // 3/12 => 1/4
  // 4/12 => 1/3
  // 3/12 => 1/4
  // 1/12 => 1/12
}

void State::endFight() {
  if (fight->getFightingCharacters(0).size() == 0) {
    Team* team = fight->getTeam(0);
    for (auto c : team->getCharacters(fight->getNb()))
      this->delCharacter(c);
    if (team->getCharacters().size() == 0)
      this->delTeam(team);
    delete fight;
    fight = nullptr;
    this->resetContents();
  }
  if (fight->getFightingCharacters(1).size() == 0) {
    Team* team = fight->getTeam(1);
    // for (c : team->getCharacters(fight->getNb())) this->delCharacter(c);
    // if (team->getCharacters().size() == 0)
    this->delTeam(team);
    delete fight;
    fight = nullptr;
    this->resetContents();
  }
}