// Generated by dia2code
#ifndef STATE__WORLD__H
#define STATE__WORLD__H

#include <vector>
#include <memory>
#include <stdlib.h>

namespace state {
  class Cell;
}

#include "Cell.h"

namespace state {

  /// class World - 
  class World {
    // Associations
    // Attributes
  private:
    int row;
    int column;
    std::vector<std::vector<std::unique_ptr<Cell>>> grid;
    // Operations
  public:
    World (int row, int column);
    std::vector<std::vector<std::unique_ptr<Cell>>> getGrid ();
    Cell& getCell (std::size_t i, std::size_t j);
    void setCell (std::size_t i, std::size_t j, Cell& cell);
    // Setters and Getters
  };

};

#endif
