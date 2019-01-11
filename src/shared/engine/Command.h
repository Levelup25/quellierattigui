// Generated by dia2code
#ifndef ENGINE__COMMAND__H
#define ENGINE__COMMAND__H

#include <string>
#include <json/json.h>

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

#include "Engine.h"
#include "state/Character.h"
#include "state/State.h"

namespace engine {

  /// class Command - 
  class Command {
    // Associations
    // Attributes
  protected:
    state::State* state;
    Engine* engine;
    state::Character* character;
    bool reverse;
    std::string type     = "Command";
    // Operations
  public:
    std::string getType ();
    void setReverse (bool reverse = true);
    virtual void execute () = 0;
    virtual void const serialize (Json::Value& out) = 0;
    static Command* deserialize (const Json::Value& in, state::State* state, engine::Engine* engine = nullptr);
    // Setters and Getters
    const state::State*& getState() const;
    void setState(const state::State*& state);
    const Engine*& getEngine() const;
    void setEngine(const Engine*& engine);
    const state::Character*& getCharacter() const;
    void setCharacter(const state::Character*& character);
    bool getReverse() const;
    void setType(const std::string& type);
  };

};

#endif
