#include "CommandsService.h"

using namespace server;
using namespace engine;

CommandsService::CommandsService(Engine* engine) : AbstractService("/commands"), engine(engine) {}

HttpStatus CommandsService::get(Json::Value& out, int id) const {}

HttpStatus CommandsService::put(Json::Value& out, const Json::Value& in) {}

HttpStatus CommandsService::remove(int id) {}
