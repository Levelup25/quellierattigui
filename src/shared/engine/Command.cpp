#include "Command.h"

using namespace std;
using namespace state;
using namespace engine;

void Command::setReverse(bool reverse) {
  this->reverse = reverse;
}

void Command::execute() {}