#include "Team.h"

namespace state {

Team::Team(std::string name) {
  this->name = name;
  inventory.reset(new Inventory());
}

}