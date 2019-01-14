/**
 * @file PlayerService.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "PlayerService.h"
#include "ServiceException.h"
using namespace server;
using namespace std;

PlayerService::PlayerService(PlayerDB& playerDB)
    : AbstractService("/player"), playerDB(playerDB) {}

HttpStatus PlayerService::get(Json::Value& out, int id) const {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  out["name"] = player->name;
  out["free"] = player->free;
  return HttpStatus::OK;
}

HttpStatus PlayerService::post(const Json::Value& in, int id) {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  unique_ptr<Player> playermod(new Player(*player));
  if (in.isMember("name")) {
    playermod->name = in["name"].asString();
  }
  if (in.isMember("free")) {
    playermod->free = in["free"].asBool();
  }
  playerDB.setPlayer(id, std::move(playermod));
  return HttpStatus::NO_CONTENT;
}

HttpStatus PlayerService::put(Json::Value& out, const Json::Value& in) {
  string name = in["name"].asString();
  int free = in["free"].asBool();
  out["id"] = playerDB.addPlayer(unique_ptr<Player>(new Player(name, free)));
  return HttpStatus::CREATED;
}

HttpStatus PlayerService::remove(int id) {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  playerDB.removePlayer(id);
  return HttpStatus::NO_CONTENT;
}
