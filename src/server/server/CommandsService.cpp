#include "CommandsService.h"
#include <iostream>
#include "ServiceException.h"

using namespace server;
using namespace state;
using namespace engine;
using namespace std;

CommandsService::CommandsService(Game* game)
    : AbstractService("/commands"), game(game) {
  // state = game->getState();
  // engine = game->getEngine();
}

HttpStatus CommandsService::get(Json::Value& out, int id) const {
  if (id >= 0) {
    Json::Value json;
    deque<Command*> commands = game->getEngine()->getCommands();
    for (int i = id; i < (int)commands.size(); i++) {
      json = Json::Value::null;
      commands[i]->serialize(json);
      if (json != Json::Value::null) {
        out.append(json);
      }
    }
  } else
    throw ServiceException(HttpStatus::BAD_REQUEST, "Cannot access element");
  return HttpStatus::OK;
}

HttpStatus CommandsService::put(Json::Value& out, const Json::Value& in) {
  out["i"] = game->getEngine()->getSize();
  game->getEngine()->addCommand(
      Command::deserialize(in, game->getState(), game->getEngine()));
  return HttpStatus::CREATED;
}

HttpStatus CommandsService::remove(int id) {
  if (id < 0)
    game->getEngine()->clearCommands();
  else if (id == 0)
    game->getEngine()->clearCommand();
  else
    throw ServiceException(HttpStatus::BAD_REQUEST, "Cannot access element");
  return HttpStatus::NO_CONTENT;
}
