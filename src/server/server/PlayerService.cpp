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

PlayerService::PlayerService(Game* game)
    : AbstractService("/players"), game(game) {
  playerDB = game->getPlayerDB();
}

HttpStatus PlayerService::get(Json::Value& out, int id) const {
  if (id < 0) {
    Json::Value json;
    for (int i = 0; i < playerDB->getIdseq(); i++) {
      const Player* player = playerDB->getPlayer(id);
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
    const Player* player = playerDB->getPlayer(id);
    if (!player)
      throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
    out["pseudo"] = player->pseudo;
    out["isLogged"] = player->isLogged;
    out["teamId"] = player->teamId;
  }
  return HttpStatus::OK;
}

HttpStatus PlayerService::post(const Json::Value& in, int id) {
  const Player* player = playerDB->getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  unique_ptr<Player> playermod(new Player(*player));
  if (in.isMember("pseudo"))
    playermod->pseudo = in.get("pseudo", "").asString();
  if (in.isMember("isLogged"))
    playermod->isLogged = in.get("isLogged", true).asBool();
  if (in.isMember("teamId"))
    playermod->teamId = in.get("teamId", -1).asInt();
  playerDB->setPlayer(id, std::move(playermod));
  return HttpStatus::OK;
}

HttpStatus PlayerService::put(Json::Value& out, const Json::Value& in) {
  static unsigned int countLog = 0;
  string pseudo = in.get("pseudo", "").asString();
  bool isLogged = in.get("isLogged", true).asBool();
  int teamId = in.get("teamId", countLog).asInt();
  for (int i = 0; i < playerDB->getIdseq(); i++) {
    const Player* player = playerDB->getPlayer(i);
    if (player) {
      if (!pseudo.compare(player->pseudo)) {
        if (player->isLogged) {
          throw ServiceException(HttpStatus::BAD_REQUEST,
                                 "Pseudo already taken");
        } else {
          out["id"] = i;
          out["pseudo"] = player->pseudo;
          out["isLogged"] = player->isLogged;
          out["teamId"] = player->teamId;
          return HttpStatus::OK;
        }
      }
    }
  }

  out["id"] = playerDB->addPlayer(
      unique_ptr<Player>(new Player(pseudo, isLogged, teamId)));
  out["pseudo"] = pseudo;
  out["isLogged"] = isLogged;
  out["teamId"] = teamId;
  game->teamIndexes.push_back(teamId);
  game->fights.push_back(nullptr);
  countLog++;
  return HttpStatus::CREATED;
}

HttpStatus PlayerService::remove(int id) {
  const Player* player = playerDB->getPlayer(id);
  if (!player)
    throw ServiceException(HttpStatus::NOT_FOUND, "Invalid player id");
  playerDB->removePlayer(id);
  return HttpStatus::NO_CONTENT;
}
