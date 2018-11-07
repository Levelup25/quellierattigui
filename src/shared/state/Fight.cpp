#include "Fight.h"

using namespace std;
using namespace state;

Fight::Fight(Team* main, Team* opponent) {
    this->main = main;
    this->opponent = opponent;
}

int Fight::getTurn() {
    return turn;
}

vector<Team*> Fight::getTeams() {
    return
    {
        main, opponent
    };
}

std::vector<Character*> Fight::getFightingCharacters() {
    std::vector<Character*> chars;
    for (auto c : main->getCharacters()) {
        if (c->getPv() > 0) chars.push_back(c);
    }
    for (auto c : opponent->getCharacters()) {
        if (c->getPv() > 0) chars.push_back(c);
    }
    return chars;
}

void Fight::endTurn() {
    for (auto c : this->getTeams()[turn % 2]->getCharacters()) {
        c->resetPm();
        c->resetPa();
    }
    turn++;
}