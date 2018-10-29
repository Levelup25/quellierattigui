// Generated by dia2code
#ifndef ENGINE__COMMAND__H
#define ENGINE__COMMAND__H

#include "AStar.h"
#include <stdlib.h>

namespace state {
  class State;
}

#include "state/State.h"

namespace engine {

  /// class Command - 
  class Command {
    // Associations
    // Attributes
  private:
    state::State* state;
    AStar::Generator generator;
    // Operations
  public:
    void setGenerator ();
    void execute (state::Character* character, std::size_t i, std::size_t j);
    // Setters and Getters
  };

};

#endif
