#include "MoveCommands.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include "FightCommand.h"
#include "MoveCommand.h"

using namespace std;
using namespace state;
using namespace engine;

MoveCommands::MoveCommands(State* state,
                           Engine* engine,
                           Character* character,
                           int i,
                           int j,
                           bool reverse) {
  this->state = state;
  this->engine = engine;
  this->character = character;
  this->i = i;
  this->j = j;
  this->reverse = reverse;
  n = state->getN();
  m = state->getM();
  generator.setWorldSize({(int)n, (int)m});
}

vector<int> MoveCommands::getDiff() {
  // return {(int)i - (int)character->getI(), (int)j - (int)character->getJ()};
  return {(int)i, (int)j};
}

void MoveCommands::setGenerator() {
  int i0 = character->getI(), j0 = character->getJ();
  int xv = (i0 / n) * n, yv = (j0 / m) * m;
  generator.clearCollisions();
  for (int l = 0; l < (int)m; l++) {
    for (int k = 0; k < (int)n; k++) {
      if (state->getCell(k + xv, l + yv)->getContent() != nothing)
        generator.addCollision({k, l});
    }
  }
}

vector<vector<int>> MoveCommands::getPath() {
  vector<vector<int>> coords;
  this->character = character;
  this->setGenerator();
  int i0 = character->getI(), j0 = character->getJ();
  i += i0, j += j0;
  int xv = (i0 / n) * n, yv = (j0 / m) * m;
  generator.removeCollision({i0 - xv, j0 - yv});
  generator.removeCollision({(int)i - xv, (int)j - yv});
  auto path =
      generator.findPath({(int)i - xv, (int)j - yv}, {i0 - xv, j0 - yv});
  path.erase(path.begin());

  if (state->getCell(i, j) && state->getCell(i, j)->getContent() > 0 &&
      path.size() > 0)
    path.pop_back();
  for (auto coord : path)
    coords.push_back({xv + coord.x, yv + coord.y});
  i -= i0, j -= j0;
  return coords;
}

void MoveCommands::execute() {
  // sf::Music music;
  if (!reverse) {
    this->character = character;
    this->setGenerator();
    int iinit = character->getI(), jinit = character->getJ();
    int i0 = iinit, j0 = jinit;
    // if (reverse) {
    //   i = -i, j = -j;
    // }
    i += i0, j += j0;
    int xv = (i0 / n) * n, yv = (j0 / m) * m;
    generator.removeCollision({i0 - xv, j0 - yv});
    generator.removeCollision({(int)i - xv, (int)j - yv});
    auto path = generator.findPath({(int)(i - xv), (int)(j - yv)},
                                   {(int)(i0 - xv), (int)(j0 - yv)});
    path.erase(path.begin());
    if ((int)state->getCell(i, j)->getContent() > 0 && path.size() > 0)
      path.pop_back();

    float step = 1.0 / 12;
    int pm;

    if (path.size() > 0 && (!state->isFighting() ||
                            (state->isFighting() &&
                             (int)path.size() <= character->getPmCurrent()))) {
      for (auto coord = path.begin(); coord != path.end(); coord++) {
        float k = (*coord).x + xv, l = (*coord).y + yv;
        for (float f = 1; f >= 0; f = f - step) {
          if (f < step)
            f = 0;
          // cout << (i0 - k) * f + k << " " << (j0 - l) * f + l << endl;
          pm = f ? 0 : 1;
          engine->addCommand(new MoveCommand(state, character, (i0 - k) * f + k,
                                             (j0 - l) * f + l, pm, reverse));
        }
        i0 = k;
        j0 = l;
      }
    }

    if (state->getCharacter(i, j) != character &&
        (int)state->getCell(i, j)->getContent() == 1 && !state->isFighting()) {
      while (character == state->getCharacter(i, j))
        ;
      engine->addCommand(
          new FightCommand(state, engine, state->getTeam(character),
                           state->getTeam(state->getCharacter(i, j))));
    } else if (state->getCell(i, j)->getContent() <= 1 &&
               !state->isFighting()) {
      if (i % n == 0 && i > 0 && state->getCell(i - 1, j)->getContent() == 0) {
        engine->addCommand(new MoveCommand(state, character, i - 1, j));
      } else if ((i + 1) % n == 0 && i + 1 < (int)state->getI() &&
                 state->getCell(i + 1, j)->getContent() == 0) {
        engine->addCommand(new MoveCommand(state, character, i + 1, j));
      } else if (j % m == 0 && j > 0 &&
                 state->getCell(i, j - 1)->getContent() == 0) {
        engine->addCommand(new MoveCommand(state, character, i, j - 1));
      } else if ((j + 1) % m == 0 && j + 1 < (int)state->getJ() &&
                 state->getCell(i, j + 1)->getContent() == 0) {
        engine->addCommand(new MoveCommand(state, character, i, j + 1));
      }
    }
    i -= iinit, j -= jinit;
  } else
    engine->addCommand(nullptr);
}

void const MoveCommands::serialize(Json::Value& out) {
  return;
  out["command"] = "MoveCommands";
  vector<Character*> characters = state->initialCharacters;
  int k = characters.size();
  while (k--)
    if (character == characters[k]) {
      out["character"] = k;
      break;
    }
  out["i"] = i;
  out["j"] = j;
}

MoveCommands* MoveCommands::deserialize(const Json::Value& in,
                                        State* state,
                                        Engine* engine) {
  Character* character =
      state->initialCharacters[in.get("character", 0).asInt()];
  size_t i = in.get("i", 0).asInt();
  size_t j = in.get("j", 0).asInt();
  return new MoveCommands(state, engine, character, i, j);
}