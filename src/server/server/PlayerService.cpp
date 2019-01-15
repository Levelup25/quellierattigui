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
  if (id < 0) {
    Json::Value json;
    for (int i = 0; i < playerDB.getIdseq(); i++) {
      const Player* player = playerDB.getPlayer(id);
      if (player) {
        json = Json::Value::null;
        json["id"] = i;
        json["pseudo"] = player->pseudo;
        json["isLogged"] = player->isLogged;
        json["teamId"] = player->teamId;
        out.append(json);
      }
    }
  } else {
    const Player* player = playerDB.getPlayer(id);
    if (!player)
      throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
    out["pseudo"] = player->pseudo;
    out["isLogged"] = player->isLogged;
    out["teamId"] = player->teamId;
  }
  return HttpStatus::OK;
}

HttpStatus PlayerService::post(const Json::Value& in, int id) {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  unique_ptr<Player> playermod(new Player(*player));
  playermod->pseudo = in.get("pseudo", "").asString();
  playermod->isLogged = in.get("isLogged", true).asBool();
  playermod->teamId = in.get("teamId", -1).asInt();
  playerDB.setPlayer(id, std::move(playermod));
  return HttpStatus::NO_CONTENT;
}

HttpStatus PlayerService::put(Json::Value& out, const Json::Value& in) {
  string pseudo = in.get("pseudo", "").asString();
  bool isLogged = in.get("isLogged", true).asBool();
  int teamId = in.get("teamId", -1).asInt();
  out["id"] = playerDB.addPlayer(
      unique_ptr<Player>(new Player(pseudo, isLogged, teamId)));
  return HttpStatus::CREATED;
}

HttpStatus PlayerService::remove(int id) {
  const Player* player = playerDB.getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  playerDB.removePlayer(id);
  return HttpStatus::NO_CONTENT;
}
