#include "CommandsService.h"
#include "ServiceException.h"

using namespace server;
using namespace state;
using namespace engine;

CommandsService::CommandsService(State& state, Engine& engine)
    : AbstractService("/commands"), state(state), engine(engine) {}

HttpStatus CommandsService::get(Json::Value& out, int id) const {
  if (id < 0) {
    Json::Value json;
    // out[0]["seed"] = state.seed;
    auto commands = engine.getCommands(-id - 1);
    for (int i = 0; i < (int)commands.size(); i++) {
      json = Json::Value::null;
      commands[i]->serialize(json);
      if (json != Json::Value::null) {
        out.append(json);
      }
    }
  } else if (id == 0)
    engine.getCommand()->serialize(out);
  else
    throw ServiceException(HttpStatus::BAD_REQUEST, "Cannot access element");
  return HttpStatus::OK;
}

HttpStatus CommandsService::put(Json::Value& out, const Json::Value& in) {
  engine.addCommand(Command::deserialize(in, &state, &engine));
  return HttpStatus::CREATED;
}

HttpStatus CommandsService::remove(int id) {
  if (id < 0)
    engine.clearCommands();
  else if (id == 0)
    engine.clearCommand();
  else
    throw ServiceException(HttpStatus::BAD_REQUEST, "Cannot access element");
  return HttpStatus::NO_CONTENT;
}
