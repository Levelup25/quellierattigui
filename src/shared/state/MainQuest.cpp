#include "MainQuest.h"

using namespace std;
using namespace state;

MainQuest::~MainQuest() {}

string MainQuest::getDescription() {
  return description;
}

bool MainQuest::checkFinished() {
  return finished;
}