/**
 * @file PlayerDB.cpp
 * @author Philippe-Henri Gosselin
 * @date 9 décembre 2015
 * @copyright CNRS
 */

#include "PlayerDB.h"
using namespace server;
using namespace std;

PlayerDB::PlayerDB() : idseq(1) {}

const Player* PlayerDB::getPlayer(int id) const {
  auto ite = players.find(id);
  if (ite == players.cend())
    return nullptr;
  return ite->second.get();
}

int PlayerDB::addPlayer(unique_ptr<Player> player) {
  int id = idseq++;
  players.insert(std::make_pair(id, std::move(player)));
  return id;
}

void PlayerDB::setPlayer(int id, unique_ptr<Player> player) {
  players[id] = std::move(player);
  if (id > idseq) {
    idseq = id;
  }
}

void PlayerDB::removePlayer(int id) {
  auto ite = players.find(id);
  if (ite == players.end())
    return;
  players.erase(ite);
}
