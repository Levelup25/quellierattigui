// Generated by dia2code
#ifndef AI__AI__H
#define AI__AI__H

#include <vector>

namespace state {
  class State;
};
namespace engine {
  class Engine;
};
namespace state {
  class Character;
};
namespace engine {
  class Command;
}

#include "engine/Engine.h"
#include "state/State.h"
#include "engine/Command.h"
#include "state/Character.h"

namespace ai {

  /// class AI - 
  class AI {
    // Associations
    // Attributes
  protected:
    state::State* state;
    engine::Engine* engine;
    // Operations
  public:
    virtual void run (state::Character* character) = 0;
  protected:
    std::vector<engine::Command*> listCommands (state::Character* character, int type = -1);
    // Setters and Getters
    const state::State*& getState() const;
    void setState(const state::State*& state);
    const engine::Engine*& getEngine() const;
    void setEngine(const engine::Engine*& engine);
  };

};

#endif
