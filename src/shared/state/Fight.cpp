#include "Fight.h"
#include <iostream>

using namespace std;
using namespace state;

Fight::Fight(Team* main, Team* opponent, int nb) {
  if (main == nullptr || opponent == nullptr)
    cout << "Error creating team" << endl;

  this->main = main;
  this->opponent = opponent;
  this->nb = nb;
  toDeploy = main->getCharacters(nb);
}

int Fight::getTurn() {
  return turn;
}

int Fight::getNb() {
  return nb;
}

Team* Fight::getTeam(size_t i) {
  if (this != nullptr) {
    if (i == 0)
      return main;
    else if (i == 1)
      return opponent;
  }
  return nullptr;
}

vector<Team*> Fight::getTeams() {
  return {main, opponent};
}

std::vector<Character*> Fight::getCharacters(int i) {
  std::vector<Character*> chars;
  if (this != nullptr) {
    if (i != 1) {
      for (auto c : main->getCharacters(nb)) {
        chars.push_back(c);
      }
    }
    if (i != 0) {
      for (auto c : opponent->getCharacters(nb)) {
        chars.push_back(c);
      }
    }
  }
  return chars;
}

std::vector<Character*> Fight::getFightingCharacters(int i) {
  std::vector<Character*> chars;
  if (this != nullptr) {
    if (i != 1 && main) {
      for (auto c : main->getCharacters(nb)) {
        if (c->getPvCurrent() > 0) {
          chars.push_back(c);
          if (turn == 0)
            if (toDeploy.size())
              for (auto c2 = toDeploy.begin() + 1; c2 != toDeploy.end(); c2++) {
                if (c == *c2) {
                  chars.pop_back();
                  break;
                }
              }
        }
      }
    }
    if (i != 0 && opponent) {
      for (auto c : opponent->getCharacters(nb)) {
        if (c->getPvCurrent() > 0)
          chars.push_back(c);
      }
    }
  }
  return chars;
}

void Fight::addTurn(int turn) {
  this->turn += turn;
  // for (auto c : this->getFightingCharacters((turn + 1) % 2)) {
  //   c->resetPm();
  //   c->resetPa();
  // }
}