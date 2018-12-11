#include "ResourceCommand.h"
#include <string.h>

using namespace std;
using namespace state;
using namespace engine;

ResourceCommand::ResourceCommand(State* state,
                                 vector<Character*> characters,
                                 vector<string> types,
                                 vector<int> values,
                                 bool reverse) {
  this->state = state;
  this->characters = characters;
  this->types = types;
  this->values = values;
  this->reverse = reverse;
}

void ResourceCommand::execute() {
  for (int i = 0; i < characters.size(); i++) {
    if (reverse)
      values[i] *= -1;
    if (!types[i].compare("pv")) {
      characters[i]->removePv(values[i]);
      if (characters[i]->getPvCurrent() <= 0)
        state->getCell(characters[i]->getI(), characters[i]->getJ())
            ->setContent(nothing);
      else
        state->getCell(characters[i]->getI(), characters[i]->getJ())
            ->setContent(perso);
    } else if (!types[i].compare("pa"))
      characters[i]->removePa(values[i]);
    else if (!types[i].compare("pm"))
      characters[i]->removePm(values[i]);
  }
}