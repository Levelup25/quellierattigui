// Generated by dia2code
#ifndef SERVER__COMMANDSSERVICE__H
#define SERVER__COMMANDSSERVICE__H

#include <json/json.h>

namespace engine {
  class Engine;
};
namespace server {
  class Game;
  class AbstractService;
}

#include "HttpStatus.h"
#include "AbstractService.h"
#include "engine/Engine.h"

namespace server {

  /// class CommandsService - 
  class CommandsService : public server::AbstractService {
    // Associations
    // Attributes
  private:
    engine::Engine* engine;
    // Operations
  public:
    CommandsService (Game* game);
    HttpStatus get (Json::Value& out, int id) const;
    HttpStatus put (Json::Value& out, const Json::Value& in);
    HttpStatus remove (int id);
    // Setters and Getters
  };

};

#endif
