// Generated by dia2code
#ifndef CLIENT__NETWORKCLIENT__H
#define CLIENT__NETWORKCLIENT__H

#include <string>
#include <json/json.h>

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

  /// class NetworkClient - 
  class NetworkClient {
    // Associations
    // Attributes
  private:
    std::string url;
    int port;
    int character;
    state::State* state;
    engine::Engine* engine;
    ai::AI* ai;
    // Operations
  public:
    NetworkClient (const std::string& url, int port, int character);
    void run ();
  protected:
    std::string getGameStatus ();
    bool getServerCommands (Json::Value& out);
    void putServerCommand (engine::Command* command);
    // Setters and Getters
  };

};

#endif
