#include "Fight.h"

using namespace std;
using namespace state;

Fight::Fight(Team* main, Team* opponent) {
  this->main = main;
  this->opponent = opponent;
}

vector<Team*> Fight::getTeams() {
  return {main, opponent};
}

std::vector<Character*> Fight::getFightingCharacters() {
  std::vector<Character*> mainTeanCharacters, opponentTeamCharacters,
      allCharacters;
  mainTeanCharacters = main->getCharacters();
  opponentTeamCharacters = opponent->getCharacters();

  allCharacters.reserve(mainTeanCharacters.size() +
                        opponentTeamCharacters.size());  // preallocate memory
  allCharacters.insert(allCharacters.end(), mainTeanCharacters.begin(),
                       mainTeanCharacters.end());
  allCharacters.insert(allCharacters.end(), opponentTeamCharacters.begin(),
                       opponentTeamCharacters.end());

  return allCharacters;
}

bool Fight::shouldEnd() {
  return !main->isAlive() or opponent->isAlive();
}