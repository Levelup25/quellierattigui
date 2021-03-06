// Generated by dia2code
#ifndef CLIENT__NETWORKCLIENT__H
#define CLIENT__NETWORKCLIENT__H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <deque>

namespace state {
  class State;
};
namespace render {
  class Render;
};
namespace engine {
  class Engine;
};
namespace ai {
  class AI;
}

#include "render/Render.h"
#include "engine/Engine.h"
#include "state/State.h"

namespace client {

  /// class NetworkClient - 
  class NetworkClient {
    // Associations
    // Attributes
  private:
    sf::Http http;
    std::string url;
    int port;
    int idLastExecutedCmd;
    state::State* state;
    render::Render* render;
    engine::Engine* engine;
    ai::AI* ai;
    std::deque<engine::Command*> commands;
    // Operations
  public:
    NetworkClient (const std::string& url, int port);
    void launch_threads (state::State* state, render::Render* render, engine::Engine* engine, ai::AI* ai);
    void run ();
  protected:
    std::deque<engine::Command*> getServerCommands ();
    void putServerCommand (engine::Command* command);
    int getGameStatus ();
    // Setters and Getters
  };

};

#endif
