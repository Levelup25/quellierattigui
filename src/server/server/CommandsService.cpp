#include "CommandsService.h"
#include <iostream>
#include "ServiceException.h"

using namespace server;
using namespace state;
using namespace engine;
using namespace std;

CommandsService::CommandsService(Game* game)
    : AbstractService("/commands"), game(game) {
  state = game->getState();
  engine = game->getEngine();
}

HttpStatus CommandsService::get(Json::Value& out, int id) const {
  if (id >= 0) {
    Json::Value json;
    deque<Command*> commands = engine->getCommands();
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
  out["i"] = engine->getSize();
  Command* cmd = Command::deserialize(in, state, engine);
  engine->addCommand(cmd);

  int id = 0;
  Team* team = state->getTeam(cmd->getCharacter());
  vector<Team*> teams = state->getTeams();
  for (int i = 0; i < (int)game->teamIndexes.size(); i++) {
    if (teams[game->teamIndexes[i]] == team) {
      id = i;
      break;
    }
  }
  state->setFight(game->fights[id]);
  cmd->execute();
  game->fights[id] = state->getFight();
  return HttpStatus::CREATED;
}

HttpStatus CommandsService::remove(int id) {
  if (id < 0)
    engine->clearCommands();
  else if (id == 0)
    engine->clearCommand();
  else
    throw ServiceException(HttpStatus::BAD_REQUEST, "Cannot access element");
  return HttpStatus::NO_CONTENT;
}
