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

std::vector<Character*> Fight::getFightingCharacters() {
    std::vector<Character*> chars;
    for (auto c : main->getCharacters()) {
        chars.push_back(c);
    }
    for (auto c : opponent->getCharacters()) {
        chars.push_back(c);
    }
    return chars;
}

void Fight::endTurn() {
    for (auto c : this->getTeam(turn % 2)->getCharacters()) {
        c->resetPm();
        c->resetPa();
    }
    turn++;
}