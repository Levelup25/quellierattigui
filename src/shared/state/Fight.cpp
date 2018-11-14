#include "Fight.h"

using namespace std;
using namespace state;

Fight::Fight(Team* main, Team* opponent, int nb) {
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
    if (i == 0) return main;
    else if (i == 1) return opponent;
    return
    {
    };
}

vector<Team*> Fight::getTeams() {
    return
    {
        main, opponent
    };
}

std::vector<Character*> Fight::getFightingCharacters(int i) {
    std::vector<Character*> chars;
    if (i != 1) {
        for (auto c : main->getCharacters(nb)) {
            if (c->getPv() > 0)chars.push_back(c);
        }
    }
    if (i != 0) {
        for (auto c : opponent->getCharacters(nb)) {
            if (c->getPv() > 0)chars.push_back(c);
        }
    }
    return chars;
}

void Fight::endTurn() {
    for (auto c : this->getFightingCharacters(turn % 2)) {
        c->resetPm();
        c->resetPa();
    }
    turn++;
}