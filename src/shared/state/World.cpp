#include "World.h"

namespace state {

World::World(std::size_t i, std::size_t j) {
  for (std::size_t l = 0; l < j; l++) {
    for (std::size_t k = 0; k < j; k++) {
      Cell* cell = new Cell();
      grid[k][l] = cell;
    }
  }
}

}  // namespace state
