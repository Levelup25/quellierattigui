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
    : AbstractService("/players"), playerDB(playerDB) {}

HttpStatus PlayerService::get(Json::Value& out, int id) const {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  out["pseudo"] = player->pseudo;
  out["isLogged"] = player->isLogged;
  return HttpStatus::OK;
}

HttpStatus PlayerService::post(const Json::Value& in, int id) {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  unique_ptr<Player> playermod(new Player(*player));
  if (in.isMember("pseudo")) {
    playermod->pseudo = in["pseudo"].asString();
  }
  if (in.isMember("isLogged")) {
    playermod->isLogged = in["isLogged"].asBool();
  }
  playerDB.setPlayer(id, std::move(playermod));
  return HttpStatus::NO_CONTENT;
}

HttpStatus PlayerService::put(Json::Value& out, const Json::Value& in) {
  string pseudo = in["pseudo"].asString();
  bool isLogged = in["isLogged"].asBool();
  out["id"] =
      playerDB.addPlayer(unique_ptr<Player>(new Player(pseudo, isLogged)));
  return HttpStatus::CREATED;
}

HttpStatus PlayerService::remove(int id) {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  playerDB.removePlayer(id);
  return HttpStatus::NO_CONTENT;
}
