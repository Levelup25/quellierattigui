// Generated by dia2code
#ifndef ENGINE__MOVECOMMANDS__H
#define ENGINE__MOVECOMMANDS__H

#include "AStar.h"
#include <stdlib.h>

namespace state {
  class State;
  class Character;
};
namespace engine {
  class Engine;
}

#include "state/State.h"
#include "Engine.h"

namespace engine {

  /// class MoveCommands - 
  class MoveCommands {
    // Associations
    // Attributes
  private:
    state::State* state;
    state::Character* character;
    Engine* engine;
    AStar::Generator generator;
    // Operations
  public:
    MoveCommands (state::State* state, state::Character* character, Engine* engine);
    void addCommands (std::size_t i, std::size_t j);
  private:
    void setGenerator ();
    // Setters and Getters
  };

};

#endif
