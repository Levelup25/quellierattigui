// Generated by dia2code
#ifndef RENDER__RENDER__H
#define RENDER__RENDER__H

#include <stdlib.h>

namespace state {
  class State;
};
namespace engine {
  class Engine;
  class MoveCommands;
};
namespace render {
  class ContentSprite;
  class CharacterSprite;
  class TileSprite;
}

#include "engine/MoveCommands.h"
#include "engine/Engine.h"
#include "ContentSprite.h"
#include "CharacterSprite.h"
#include "TileSprite.h"
#include "state/State.h"

namespace render {

  /// class Render - 
  class Render {
    // Associations
    // Attributes
  private:
    state::State* state;
    engine::Engine* engine;
    engine::MoveCommands* mvcmd;
    std::size_t n     = 12;
    std::size_t m     = 12;
    // Operations
  public:
    Render (state::State* state, engine::Engine* engine);
    void display ();
    // Setters and Getters
  };

};

#endif