// Generated by dia2code
#ifndef ENGINE__ATTACKCOMMAND__H
#define ENGINE__ATTACKCOMMAND__H

#include <vector>
#include <stdlib.h>

namespace engine {
  class Engine;
};
namespace state {
  class Ability;
  class State;
  class Character;
};
namespace engine {
  class Command;
}

#include "Command.h"
#include "state/Ability.h"
#include "Engine.h"

namespace engine {

  /// class AttackCommand - 
  class AttackCommand : public engine::Command {
    // Associations
    // Attributes
  private:
    Engine* engine;
    int abilityNumber;
    std::vector<int> position;
    std::vector<std::vector<int>> targets;
    std::vector<std::vector<int>> effects;
    state::Ability* ability;
    // Operations
  public:
    AttackCommand (state::State* state, Engine* engine, state::Character* character, std::vector<int> position, int abilityNumber = 0);
    int getAbilityNumber ();
    void setZones (bool cut = true);
    std::vector<std::vector<int>> getZone (std::size_t i, bool cut = true);
    void execute ();
    // Setters and Getters
  };

};

#endif
