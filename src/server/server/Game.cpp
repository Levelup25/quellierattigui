#include "Game.h"
#include <fstream>

using namespace server;
using namespace state;
using namespace engine;
using namespace std;

Game::Game() {}

PlayerDB& Game::getPlayerDB() {
  return ref(playerDB);
}

State& Game::getState() {
  return ref(state);
}

Engine& Game::getEngine() {
  return ref(engine);
}

void Game::run() {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  file.open("res/weapons.txt");
  reader.parse(file, root);
  int nb = root.size();
  file.close();

  int N = state.getI(), M = state.getJ();
  int idseq, idseq_prev = -1;
  while (1) {
    idseq = playerDB.getIdseq();
    if (idseq != idseq_prev) {
      idseq_prev = idseq;
      int index = state.getTeams().size();
      for (int j = 0; j < 4; j++) {
        state.addCharacter(index, rand() % (12 * 4), (Direction)(rand() % 4),
                           N / 4 + rand() % (N / 2), M / 4 + rand() % (M / 2));
        Character* c = state.getCharacters().back();
        c->setPm(2 + rand() % 5);
        c->setPv(1 + rand() % 4);
        c->setPa(3 + rand() % 2);
        Weapon* w = new Weapon(rand() % nb);
        c->setWeapon(w);
        for (auto a : w->getAbilities()) {
          int r3 = rand() % 3;
          for (int i = 0; i < r3; i++)
            a->addLv();
        }
      }
      // state.mainTeamIndex = state.getTeams().size() - 1;
    }
    engine.runCommand();
  }
}
