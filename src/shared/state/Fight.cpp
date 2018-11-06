#include "Fight.h"

using namespace std;
using namespace state;

Fight::Fight(Team* main, Team* opponent) {
    this->main = main;
    this->opponent = opponent;
}

vector<Team*> Fight::getTeams() {
    return
    {
        main, opponent
    };
}

std::vector<Character*> Fight::getFightingCharacters() {
    std::vector<Character*> chars;
    for (auto c : main->getCharacters()) chars.push_back(c);
    for (auto c : opponent->getCharacters()) chars.push_back(c);
    return chars;
    //    std::vector<Character*> mainTeamCharacters, opponentTeamCharacters,
    //            allCharacters;
    //    mainTeamCharacters = main->getCharacters();
    //    opponentTeamCharacters = opponent->getCharacters();
    //
    //    allCharacters.reserve(mainTeamCharacters.size() +
    //            opponentTeamCharacters.size());
    //    allCharacters.insert(allCharacters.end(), mainTeamCharacters.begin(),
    //            mainTeamCharacters.end());
    //    allCharacters.insert(allCharacters.end(), opponentTeamCharacters.begin(),
    //            opponentTeamCharacters.end());
    //
    //    return allCharacters;
}

bool Fight::shouldEnd() {
    return !(main->isAlive() or opponent->isAlive());
}