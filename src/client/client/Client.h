// Generated by dia2code
#ifndef CLIENT__CLIENT__H
#define CLIENT__CLIENT__H


namespace state {
  class State;
};
namespace engine {
  class Engine;
};
namespace ai {
  class AI;
};
namespace render {
  class Render;
}

#include "render/Render.h"
#include "state/State.h"
#include "ai/AI.h"
#include "engine/Engine.h"

namespace client {

  /// class Client - 
  class Client {
    // Associations
    // Attributes
  private:
    state::State* state;
    engine::Engine* engine;
    ai::AI* ai;
    // Operations
  public:
    Client ();
    void run ();
    // Setters and Getters
  };

};

#endif