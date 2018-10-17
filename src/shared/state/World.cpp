#include "World.h"

namespace state {

    World::World(std::size_t i, std::size_t j) {
      grid.resize(i);
      for (std::size_t k = 0; k < i; k++) {
          grid[k].resize(j);
      }
      for (std::size_t l = 0; l < j; l++) {
        for (std::size_t k = 0; k < j; k++) {
          Cell* cell = new Cell();
          grid[k][l] = cell;
        }
      }
    }
    
    std::vector<std::vector<Cell*>> World::getGrid () {
        return grid;
    }
    
    Cell* World::getCell (std::size_t i, std::size_t j) {
        return grid[i][j];
    }
    
    void World::setCell (std::size_t i, std::size_t j, Cell* cell) {
        grid[i][j]=cell;
    }
    
    Character* World::getCharacter (std::size_t i, std::size_t j) {
        for (std::vector<Character*>::iterator c = characters.begin() ; c != characters.end(); ++c) {
            if (c->i==i && *c->j==j) return c;
        }
        return nullptr;
    }
    //std::vector<Character*> World::getCharacters ();

}  // namespace state