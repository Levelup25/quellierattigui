#include "MoveCommands.h"
#include <iostream>
#include "FightCommand.h"
#include "MoveCommand.h"

using namespace std;
using namespace state;
using namespace engine;

MoveCommands::MoveCommands(State* state,
                           Engine* engine,
                           Character* character,
                           size_t i,
                           size_t j) {
  this->state = state;
  this->engine = engine;
  this->character = character;
  this->i = i;
  this->j = j;
  n = state->getN();
  m = state->getM();
  generator.setWorldSize({(int)n, (int)m});
}

vector<int> MoveCommands::getDiff() {
  return {(int)i - (int)character->getI(), (int)j - (int)character->getJ()};
}

void MoveCommands::setGenerator() {
  // state->resetContents();
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
  int xv = (i0 / n) * n, yv = (j0 / m) * m;
  generator.removeCollision({i0 - xv, j0 - yv});
  generator.removeCollision({(int)i - xv, (int)j - yv});
  auto path =
      generator.findPath({(int)i - xv, (int)j - yv}, {i0 - xv, j0 - yv});
  path.erase(path.begin());

  if (state->getCell(i, j)->getContent() > 0 && path.size() > 0)
    path.pop_back();
  for (auto coord : path)
    coords.push_back({xv + coord.x, yv + coord.y});
  return coords;
}

void MoveCommands::execute() {
  this->character = character;
  this->setGenerator();
  int i0 = character->getI(), j0 = character->getJ();
  int xv = (i0 / n) * n, yv = (j0 / m) * m;
  generator.removeCollision({i0 - xv, j0 - yv});
  generator.removeCollision({(int)i - xv, (int)j - yv});
  auto path = generator.findPath({(int)(i - xv), (int)(j - yv)},
                                 {(int)(i0 - xv), (int)(j0 - yv)});
  path.erase(path.begin());
  if ((int)state->getCell(i, j)->getContent() > 0 && path.size() > 0)
    path.pop_back();

  float step = 1.0 / 12;

  if (path.size() > 0 &&
      (!state->isFighting() ||
       (state->isFighting() && path.size() <= character->getPmCurrent()))) {
    if (state->isFighting())
      character->removePm(path.size());
    for (auto coord = path.begin(); coord != path.end(); coord++) {
      float k = (*coord).x + xv, l = (*coord).y + yv;
      for (float f = 1 - step; f >= 0; f = f - step) {
        if (f < step)
          f = 0;
        // cout << (i0 - k) * f + k << " " << (j0 - l) * f + l << endl;
        engine->addCommand(new MoveCommand(state, character, (i0 - k) * f + k,
                                           (j0 - l) * f + l));
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
        new FightCommand(state, state->getTeam(character),
                         state->getTeam(state->getCharacter(i, j))));
  } else if (state->getCell(i, j)->getContent() <= 1 && !state->isFighting()) {
    if (i % n == 0 && i > 0 && state->getCell(i - 1, j)->getContent() == 0) {
      engine->addCommand(new MoveCommand(state, character, i - 1, j));
    } else if ((i + 1) % n == 0 && i + 1 < state->getI() &&
               state->getCell(i + 1, j)->getContent() == 0) {
      engine->addCommand(new MoveCommand(state, character, i + 1, j));
    } else if (j % m == 0 && j > 0 &&
               state->getCell(i, j - 1)->getContent() == 0) {
      engine->addCommand(new MoveCommand(state, character, i, j - 1));
    } else if ((j + 1) % m == 0 && j + 1 < state->getJ() &&
               state->getCell(i, j + 1)->getContent() == 0) {
      engine->addCommand(new MoveCommand(state, character, i, j + 1));
    }
  }
}