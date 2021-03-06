// Generated by dia2code
#ifndef ENGINE__DAMAGECOMMAND__H
#define ENGINE__DAMAGECOMMAND__H

#include <vector>
#include <json/json.h>

namespace state {
  class Ability;
  class State;
};
namespace engine {
  class Engine;
};
namespace state {
  class Character;
};
namespace engine {
  class DamageCommand;
  class Command;
}

#include "Command.h"
#include "state/Ability.h"

namespace engine {

  /// class DamageCommand - 
  class DamageCommand : public engine::Command {
    // Associations
    // Attributes
  private:
    std::vector<std::vector<int>> positions;
    std::vector<int> directions;
    int dmg;
    int abilityNumber;
    state::Ability* ability;
    // Operations
  public:
    DamageCommand (state::State* state, Engine* engine, state::Character* character, std::vector<std::vector<int>> positions, std::vector<int> directions, int abilityNumber = 0, int dmg = 0, bool reverse = false);
    void execute ();
    void const serialize (Json::Value& out);
    static DamageCommand* deserialize (const Json::Value& in, state::State* state, engine::Engine* engine = nullptr);
    // Setters and Getters
  };

};

#endif
