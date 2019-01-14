#include "GameService.h"

using namespace server;

GameService::GameService(Game* game) : AbstractService("/game"), game(game) {}

HttpStatus GameService::get(Json::Value& out, int id) const {}