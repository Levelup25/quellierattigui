#include "Fight.h"

using namespace std;
using namespace state;

Fight::Fight(Team* main, Team* opponent) {
    this->main = main;
    this->opponent = opponent;
}