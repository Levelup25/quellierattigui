#include "DamageCommand.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "FightCommand.h"

using namespace std;
using namespace state;
using namespace engine;

DamageCommand::DamageCommand(State* state,
                             Engine* engine,
                             Character* character,
                             vector<vector<int>> positions,
                             vector<int> directions,
                             int abilityNumber,
                             int dmg,
                             bool reverse) {
  this->state = state;
  this->engine = engine;
  this->character = character;
  this->positions = positions;
  this->directions = directions;
  this->abilityNumber = abilityNumber;
  this->dmg = dmg;
  this->reverse = reverse;
}

void DamageCommand::execute() {
  ability = character->getWeapon()->getAbility(abilityNumber);
  shared_ptr<Fight> fight = state->getFight();
  state->animations.clear();
  vector<Character*> fighters;
  int i = 0;
  bool heal = ability->getDamage() <= 0;
  for (auto pos : positions) {
    if (directions[0] != -1)
      state->animations.push_back({pos[0], pos[1], directions[i++],
                                   ability->getElement(), ability->getLv(),
                                   heal});
    if (dmg != 0) {
      Character* c = state->getCharacter(pos[0], pos[1]);
      if (!reverse)
        fighters = fight->getFightingCharacters();
      else {
        fighters = fight->getCharacters();
      }
      if (find(fighters.begin(), fighters.end(), c) != fighters.end()) {
        if (!reverse)
          c->removePv(ability->getDamage(state, character, c));
        else
          c->removePv(-ability->getDamage(state, character, c));

        if (c->getPvCurrent() <= 0)
          state->getCell(c->getI(), c->getJ())->setContent(nothing);
        else
          state->getCell(c->getI(), c->getJ())->setContent(perso);
      }
    }
  }

  if (!dmg && fight &&
      (fight->getFightingCharacters(0).size() == 0 ||
       fight->getFightingCharacters(1).size() == 0)) {
    unsigned int seed;
    ifstream ifile;
    Json::Value root;
    Json::Reader reader;
    ifile.open("replay.txt");
    reader.parse(ifile, root);
    seed = root[0].get("seed", 0).asUInt();
    ifile.close();
    if (seed != state->seed)
      root = Json::Value::null;

    deque<Command*> commands = engine->getCommands(true);
    ofstream file;
    file.open("replay.txt", ios::trunc);
    Json::Value json;
    if (root == Json::Value::null)
      root[0]["seed"] = state->seed;
    while (commands.size()) {
      if (commands.front()) {
        json = Json::Value::null;
        commands.front()->serialize(json);
        if (json != Json::Value::null) {
          root.append(json);
        }
      }
      commands.pop_front();
    }
    Json::StyledWriter writer;
    file << writer.write(root);
    file.close();
    state->endFight();
    engine->clearCommands(true);
  }
}

void const DamageCommand::serialize(Json::Value& out) {
  // return;
  out["command"] = "DamageCommand";
  vector<Character*> characters = state->initialCharacters;
  int k = characters.size();
  while (k--)
    if (character == characters[k]) {
      out["character"] = k;
      break;
    }
  for (int k = 0; k < (int)positions.size(); k++)
    out["ipos"].append(positions[k][0]);
  for (int k = 0; k < (int)positions.size(); k++)
    out["jpos"].append(positions[k][1]);
  for (int k = 0; k < (int)directions.size(); k++)
    out["directions"].append(directions[k]);
  out["abilityNumber"] = abilityNumber;
  out["dmg"] = dmg;
}

DamageCommand* DamageCommand::deserialize(const Json::Value& in,
                                          State* state,
                                          Engine* engine) {
  Character* character =
      state->initialCharacters[in.get("character", 0).asInt()];
  vector<vector<int>> positions;
  vector<int> ipos, jpos;
  int size = in.get("ipos", 0).size();
  for (int k = 0; k < size; k++)
    ipos.push_back(in.get("ipos", 0)[k].asInt());
  for (int k = 0; k < size; k++)
    jpos.push_back(in.get("jpos", 0)[k].asInt());
  for (int k = 0; k < size; k++)
    positions.push_back({ipos[k], jpos[k]});
  vector<int> directions;
  for (int k = 0; k < (int)in.get("directions", 0).size(); k++)
    directions.push_back(in.get("directions", 0)[k].asInt());
  int abilityNumber = in.get("abilityNumber", 0).asInt();
  int dmg = in.get("dmg", 0).asInt();
  return new DamageCommand(state, engine, character, positions, directions,
                           abilityNumber, dmg);
}