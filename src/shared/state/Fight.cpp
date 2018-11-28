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

std::vector<Character*> Fight::getFightingCharacters(int i) {
  std::vector<Character*> chars;
  if (this != nullptr) {
    if (i != 1) {
      for (auto c : main->getCharacters(nb)) {
        if (c->getPvCurrent() > 0)
          chars.push_back(c);
      }
    }
    if (i != 0) {
      for (auto c : opponent->getCharacters(nb)) {
        if (c->getPvCurrent() > 0)
          chars.push_back(c);
      }
    }
  }
  return chars;
}

void Fight::endTurn() {
  for (auto c : this->getFightingCharacters(turn++ % 2)) {
    c->resetPm();
    c->resetPa();
  }
}