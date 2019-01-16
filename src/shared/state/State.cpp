#include "State.h"
#include <json/json.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;
using namespace state;

State::State(unsigned int seed, size_t i, size_t j) {
  this->seed = seed;
  srand(seed);
  // Initialize the grid with the good dimension and fill them with random cells
  I = i;
  J = j;

  vector<ElementType> vector{wind, fire, earth, water};
  random_shuffle(vector.begin(), vector.end());
  vector.push_back(neutral);
  zones = vector;
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
          cell->setElement(vector[0]);
        } else if (k >= I / 2 && l < J / 2) {
          cell->setElement(vector[1]);
        } else if (k < I / 2 && l >= J / 2) {
          cell->setElement(vector[2]);
        } else {
          cell->setElement(vector[3]);
        }
      }
      grid[k][l] = cell;
    }
  }
  init();
}

void State::init() {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  file.open("res/weapons.txt");
  reader.parse(file, root);
  int nb = root.size();
  file.close();

  int nbteams = 20;
  // int n = getN(), m = getM();
  int N = getI(), M = getJ();

  for (int i = 0; i < nbteams; i++) {
    for (int j = 0; j < 3; j++) {
      addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4), rand() % N,
                   rand() % M);
      Character* c = getCharacters().back();
      c->setPm(2 + rand() % 5);
      c->setPv(1 + rand() % 4);
      c->setPa(3 + rand() % 2);
      Weapon* w = new Weapon(rand() % nb);
      c->setWeapon(w);
      for (auto a : w->getAbilities()) {
        int r3 = rand() % 3;
        for (int i = 0; i < r3; i++)
          a->addLv();
      }
    }
  }

  vector<string> bossnames = {"Inconnu",        "Overlord",   "Human Slayer",
                              "Mon ventre",     "Levi Djinn", "Dominatrix",
                              "Fausse chieuse", "Demon Niac"};
  // eau : 6+7   feu : 4+5   terre : 2+3   air : 0+1
  vector<int> elems = {1, 3, 2, 4};
  for (int i = 0; i < 4; i++) {
    int xb = 3 * (i % 2) * N / 4, yb = 3 * (i / 2) * M / 4, xe = xb + N / 4,
        ye = yb + M / 4;
    for (int j = 0; j < 3; j++) {
      addCharacter(i + nbteams, -1 - (9 - 2 * elems[zones[i] - 1] - (j == 0)),
                   (Direction)(rand() % 4), xb + rand() % (xe - xb),
                   yb + rand() % (ye - yb));
      Character* c = getCharacters().back();
      c->setName(bossnames[9 - 2 * elems[zones[i] - 1] - (j == 0)]);
      c->setPm(3 + 3 * (j == 0));
      c->setPv(3 + 6 * (j == 0));
      c->setPa(3 + 6 * (j == 0));
      Weapon* w = new Weapon(6 * (rand() % 3) + (j ? zones[i] : 5));
      c->setWeapon(w);
      for (auto a : w->getAbilities()) {
        int r3 = 2 + (j ? rand() % 1 : 1);
        for (int i = 0; i < r3; i++)
          a->addLv();
      }
    }
  }

  for (int j = 0; j < 4; j++) {
    addCharacter(nbteams + 4, rand() % (12 * 4), (Direction)(rand() % 4),
                 N / 4 + rand() % (N / 2), M / 4 + rand() % (M / 2));
    Character* c = getCharacters().back();
    c->setPm(2 + rand() % 5);
    c->setPv(1 + rand() % 4);
    c->setPa(3 + rand() % 2);
    Weapon* w = new Weapon(rand() % nb);
    c->setWeapon(w);
    for (auto a : w->getAbilities()) {
      int r3 = rand() % 3;
      for (int i = 0; i < r3; i++)
        a->addLv();
    }
  }
  mainTeamIndex = getTeams().size() - 1;
  // initialCharacters = getCharacters();
}

void State::resetContents() {
  int i0 = ((int)this->getMainCharacter()->getI() / n) * n,
      j0 = ((int)this->getMainCharacter()->getJ() / m) * m;
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
  initialCharacters.push_back(character);
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
  size_t i0 = i, j0 = j, i2 = i, j2 = j;
  while (grid[i2][j2]->getContent() != nothing) {
    i2++;
    if (i2 % n == 0) {
      i2 -= n;
      j2++;
      if (j2 % m == 0) {
        j2 -= m;
        // i2 += n;
      }
    }
    if (i0 == i2 && j0 == j2) {
      i2 += n;
      i0 = i2;
    }
    if (i2 >= I) {
      i2 -= I;
      j2 += m;
      if (j2 >= J) {
        j2 -= J;
      }
    }
    if (i == i2 && j == j2)
      return;
  }
  character->setI(i2);
  character->setJ(j2);
  team->addCharacter(character);
  initialCharacters.push_back(character);
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
  for (int i = 0; i < (int)teams.size(); i++)
    chars.push_back(teams[i]->getMainCharacter());
  return chars;
}

vector<Character*> State::getCharacters() {
  vector<Character*> chars;
  for (int i = 0; i < (int)teams.size(); i++) {
    vector<Character*> characters = teams[i]->getCharacters();
    for (int j = 0; j < (int)characters.size(); j++)
      chars.push_back(characters[j]);
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
  return teams[mainTeamIndex];
}

void State::delTeam(Team* team) {
  for (int i = 0; i < (int)teams.size(); i++) {
    if (teams[i] == team) {
      teams.erase(teams.begin() + i);
      // delete team;
      // team = nullptr;
      if (i <= mainTeamIndex)
        mainTeamIndex--;
      return;
    }
  }
}

Character* State::getMainCharacter() {
  return teams[mainTeamIndex]->getMainCharacter();
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

shared_ptr<Fight> State::getFight() {
  return fight;
}

void State::setFight(shared_ptr<Fight> fight) {
  this->fight = fight;
}

bool State::isFighting() {
  return fight != nullptr;
}

void State::endFight() {
  if (fight->getTurn() <= 0) {
    fight = nullptr;
    this->resetContents();
  } else {
    bool end = false;
    if (fight->getFightingCharacters(0).size() == 0) {
      Team* team = fight->getTeam(0);
      for (auto c : team->getCharacters(fight->getNb()))
        this->delCharacter(c);
      if (team->getCharacters().size() == 0) {
        this->delTeam(team);
        cout << endl << "Perdu, vous ferez mieux la prochaine fois!" << endl;
        cout << endl;
        vector<string> programmers = {"Quellier Louis", "Attigui Youness"};
        cout << "                 Credits : " << endl;
        cout << "Game designer :            " << programmers[rand() % 2]
             << endl;
        cout << "Directeur artistique :     " << programmers[rand() % 2]
             << endl;
        cout << "Développeur informatique : " << programmers[rand() % 2]
             << endl;
        cout << "Level designer :           " << programmers[rand() % 2]
             << endl;
        cout << "Infographiste :            " << programmers[rand() % 2]
             << endl;
        cout << "Testeur :                  " << programmers[rand() % 2]
             << endl;
        cout << "Sound designer :           " << programmers[rand() % 2]
             << endl;
        exit(0);
      }
      end = true;
    }

    if (fight->getFightingCharacters(1).size() == 0) {
      Team* team = fight->getTeam(1);
      // for (c : team->getCharacters(fight->getNb()))
      //   this->delCharacter(c);
      // if (team->getCharacters().size() == 0)
      if (team->getMainCharacter()->getId() < 0) {
        cout << endl
             << "Bravo vous avez vaincu " << team->getMainCharacter()->getName()
             << ", l'un des 4 seigneurs élémentaires, accompagné de 2 "
             << team->getCharacter(1)->getName() << endl;
        cout << "Saurez vous vaincre les 3 autres?..." << endl;
        cout << endl;
        vector<string> programmers = {"Quellier Louis", "Attigui Youness"};
        cout << "                 Credits : " << endl;
        cout << "Game designer :            " << programmers[rand() % 2]
             << endl;
        cout << "Directeur artistique :     " << programmers[rand() % 2]
             << endl;
        cout << "Développeur informatique : " << programmers[rand() % 2]
             << endl;
        cout << "Level designer :           " << programmers[rand() % 2]
             << endl;
        cout << "Infographiste :            " << programmers[rand() % 2]
             << endl;
        cout << "Testeur :                  " << programmers[rand() % 2]
             << endl;
        cout << "Sound designer :           " << programmers[rand() % 2]
             << endl;
        exit(0);
      }
      this->delTeam(team);
      end = true;
    }

    if (end) {
      fight = nullptr;
      this->resetContents();
    }
  }
}