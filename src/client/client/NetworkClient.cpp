#include "NetworkClient.h"
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <thread>

using namespace client;
using namespace engine;
using namespace state;
using namespace render;
using namespace ai;
using namespace std;
using namespace sf;

NetworkClient::NetworkClient(const string& url, int port) {
  http.setHost(url, port);
  actualcmd = 0;
}

void NetworkClient::state_init(State* state) {
  ifstream file;
  Json::Reader reader;
  Json::Value root;
  file.open("res/weapons.txt");
  reader.parse(file, root);
  int nb = root.size();
  file.close();

  int nbteams = 20;
  // int n = state->getN(), m = state->getM();
  int N = state->getI(), M = state->getJ();

  for (int i = 0; i < nbteams; i++) {
    for (int j = 0; j < 3; j++) {
      state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
                          rand() % N, rand() % M);
      Character* c = state->getCharacters().back();
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
  }

  vector<string> bossnames = {"Inconnu",        "Overlord",   "Human Slayer",
                              "Mon ventre",     "Levi Djinn", "Dominatrix",
                              "Fausse chieuse", "Demon Niac"};
  // eau : 6+7   feu : 4+5   terre : 2+3   air : 0+1
  vector<int> elems = {1, 3, 2, 4};
  for (int i = 0; i < 4; i++) {
    int xb = 3 * (i % 2) * N / 4, yb = 3 * (i / 2) * M / 4, xe = xb + N / 4,
        ye = yb + M / 4;
    for (int j = 0; j < 3; j++) {
      state->addCharacter(i + nbteams,
                          -1 - (9 - 2 * elems[state->zones[i] - 1] - (j == 0)),
                          (Direction)(rand() % 4), xb + rand() % (xe - xb),
                          yb + rand() % (ye - yb));
      Character* c = state->getCharacters().back();
      c->setName(bossnames[9 - 2 * elems[state->zones[i] - 1] - (j == 0)]);
      c->setPm(3 + 3 * (j == 0));
      c->setPv(3 + 6 * (j == 0));
      c->setPa(3 + 6 * (j == 0));
      Weapon* w = new Weapon(6 * (rand() % 3) + (j ? state->zones[i] : 5));
      c->setWeapon(w);
      for (auto a : w->getAbilities()) {
        int r3 = 2 + (j ? rand() % 1 : 1);
        for (int i = 0; i < r3; i++)
          a->addLv();
      }
    }
  }

  for (int j = 0; j < 4; j++) {
    state->addCharacter(nbteams + 4, rand() % (12 * 4), (Direction)(rand() % 4),
                        N / 4 + rand() % (N / 2), M / 4 + rand() % (M / 2));
    Character* c = state->getCharacters().back();
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
  state->mainTeamIndex = state->getTeams().size() - 1;
  // state->initialCharacters = state->getCharacters();
}

void NetworkClient::launch_threads(State* state,
                                   Render* render,
                                   Engine* engine,
                                   AI* ai) {
  bool end = false;
  thread t1([render, &end]() {
    Music theme;
    theme.openFromFile("res/sounds/theme.wav");
    // theme.play();
    theme.setLoop(true);
    theme.setVolume(50.f);
    render->display();
    theme.stop();
    end = true;
  });
  thread t2([engine, state, &end, this]() {
    SoundBuffer win_buffer;
    win_buffer.loadFromFile("res/sounds/win.ogg");
    Sound win_sound;
    win_sound.setBuffer(win_buffer);

    SoundBuffer lose_buffer;
    lose_buffer.loadFromFile("res/sounds/lose.ogg");
    Sound lose_sound;
    lose_sound.setBuffer(lose_buffer);

    vector<string> sounds = {"grass1", "snow1", "stone1", "gravel1", "sand1"};
    vector<SoundBuffer> move_buffers;
    move_buffers.resize(5);
    vector<Sound> move_sounds;
    move_sounds.resize(5);
    for (int i = 0; i < 5; i++) {
      move_buffers[i].loadFromFile("res/sounds/" + sounds[i] + ".ogg");
      move_sounds[i].setBuffer(move_buffers[i]);
    }

    sounds = {"explode1", "explode2", "explode3", "explode4", "explode5"};
    vector<SoundBuffer> attack_buffers;
    attack_buffers.resize(5);
    vector<Sound> attack_sounds;
    attack_sounds.resize(5);
    for (int i = 0; i < 5; i++) {
      attack_buffers[i].loadFromFile("res/sounds/" + sounds[i] + ".ogg");
      attack_sounds[i].setBuffer(attack_buffers[i]);
    }

    Clock clock;
    while (!end) {
      Command* cmd = engine->getCommand();
      if (cmd)
        putServerCommand(cmd);
      if (clock.getElapsedTime().asSeconds() >= 1.0 / 30) {
        clock.restart();
        Command* cmd = engine->getCommand();
        Character* character = nullptr;
        Team* team = nullptr;
        if (cmd)
          character = cmd->getCharacter();
        string type = cmd->getType();
        if (!type.compare("MoveCommand")) {
          int i = state->getCell(character->getI(), character->getJ())
                      ->getElement();
          if (move_sounds[i].getStatus() != 2) {
            move_sounds[i].play();
          }
        } else if (!type.compare("AttackCommand")) {
          int i = character->getWeapon()
                      ->getAbility(
                          static_cast<AttackCommand*>(cmd)->getAbilityNumber())
                      ->getElement();
          if (attack_sounds[i].getStatus() != 2) {
            attack_sounds[i].play();
          }
        } else if (!type.compare("FightCommand") && state->getFight()) {
          team = state->getFight()->getTeam(1);
        }
        // engine->runCommand();
        if (!type.compare("FightCommand") && !state->getFight()) {
          vector<Team*> teams = state->getTeams();
          if (find(teams.begin(), teams.end(), team) != teams.end()) {
            lose_sound.play();
            cout << "lose" << endl;
          } else {
            win_sound.play();
            cout << "win" << endl;
          }
        }
      }
    }
  });
  thread t3([ai, state, engine, &end]() {
    while (!end) {
      shared_ptr<Fight> fight = state->getFight();
      vector<Character*> maincharacters = state->getMainCharacters();
      if (state->two_ai && !fight && maincharacters.size() > 1 &&
          state->getMainCharacter()) {
        int x0 = state->getMainCharacter()->getI(),
            y0 = state->getMainCharacter()->getJ();
        int min = abs(maincharacters[0]->getI() - x0) +
                  abs(maincharacters[0]->getJ() - y0),
            imin = 1;
        for (int i = 1; i < (int)maincharacters.size(); i++) {
          int dist = abs(maincharacters[i]->getI() - x0) +
                     abs(maincharacters[i]->getJ() - y0);
          if (dist && dist < min) {
            min = dist;
            imin = i;
          }
        }
        int n = state->getN(), m = state->getM();
        int x1 = maincharacters[imin]->getI(),
            y1 = maincharacters[imin]->getJ();
        int xv0 = x0 / n, yv0 = y0 / m, xv1 = x1 / n, yv1 = y1 / m;
        while (!end && (xv0 != xv1 || yv0 != yv1)) {
          if (xv0 < xv1) {
            for (int y = 0; y < max(yv0 * m + m - y0, y0 - yv0 * m); y++) {
              if (y0 + y > yv0 * m && y0 + y < (yv0 + 1) * m) {
                if (state->getCell(n * (xv0 + 1) - 1, y0 + y)->getContent() ==
                        0 &&
                    state->getCell(n * (xv0 + 1), y0 + y)->getContent() == 0) {
                  engine->addCommand(
                      new MoveCommands(state, engine, state->getMainCharacter(),
                                       n * (xv0 + 1) - 1 - x0, y));
                  x0 = n * (xv0 + 1), y0 = y0 + y;
                  break;
                } else if (state->getCell(n * (xv0 + 1) - 1, y0 - y)
                                   ->getContent() == 0 &&
                           state->getCell(n * (xv0 + 1), y0 - y)
                                   ->getContent() == 0) {
                  engine->addCommand(
                      new MoveCommands(state, engine, state->getMainCharacter(),
                                       n * (xv0 + 1) - 1 - x0, -y));
                  x0 = n * (xv0 + 1), y0 = y0 - y;
                  break;
                }
              }
            }
          } else if (xv0 > xv1) {
            for (int y = 0; y < max(yv0 * m + m - y0, y0 - yv0 * m); y++) {
              if (y0 + y > yv0 * m && y0 + y < (yv0 + 1) * m) {
                if (state->getCell(n * xv0, y0 + y)->getContent() == 0 &&
                    state->getCell(n * xv0 - 1, y0 + y)->getContent() == 0) {
                  engine->addCommand(new MoveCommands(state, engine,
                                                      state->getMainCharacter(),
                                                      n * xv0 - x0, y));
                  x0 = n * xv0 - 1, y0 = y0 + y;
                  break;
                } else if (state->getCell(n * xv0, y0 - y)->getContent() == 0 &&
                           state->getCell(n * xv0 - 1, y0 - y)->getContent() ==
                               0) {
                  engine->addCommand(new MoveCommands(state, engine,
                                                      state->getMainCharacter(),
                                                      n * xv0 - x0, -y));
                  x0 = n * xv0 - 1, y0 = y0 - y;
                  break;
                }
              }
            }
          }
          while (!end && engine->getSize())
            ;
          if (yv0 < yv1) {
            for (int x = 0; x < max(xv0 * n + n - x0, x0 - xv0 * n); x++) {
              if (x0 + x > xv0 * n && x0 + x < (xv0 + 1) * n) {
                if (state->getCell(x0 + x, m * (yv0 + 1) - 1)->getContent() ==
                        0 &&
                    state->getCell(x0 + x, m * (yv0 + 1))->getContent() == 0) {
                  engine->addCommand(
                      new MoveCommands(state, engine, state->getMainCharacter(),
                                       x, m * (yv0 + 1) - 1 - y0));
                  x0 = x0 + x, y0 = m * (yv0 + 1);
                  break;
                } else if (state->getCell(x0 - x, m * (yv0 + 1) - 1)
                                   ->getContent() == 0 &&
                           state->getCell(x0 - x, m * (yv0 + 1))
                                   ->getContent() == 0) {
                  engine->addCommand(
                      new MoveCommands(state, engine, state->getMainCharacter(),
                                       -x, m * (yv0 + 1) - 1 - y0));
                  x0 = x0 - x, y0 = m * (yv0 + 1);
                  break;
                }
              }
            }
          } else if (yv0 > yv1) {
            for (int x = 0; x < max(xv0 * n + n - x0, x0 - xv0 * n); x++) {
              if (x0 + x > xv0 * n && x0 + x < (xv0 + 1) * n) {
                if (state->getCell(x0 + x, m * yv0)->getContent() == 0 &&
                    state->getCell(x0 + x, m * yv0 - 1)->getContent() == 0) {
                  engine->addCommand(new MoveCommands(state, engine,
                                                      state->getMainCharacter(),
                                                      x, m * yv0 - y0));
                  x0 = x0 + x, y0 = m * yv0 - 1;
                  break;
                } else if (state->getCell(x0 - x, m * yv0)->getContent() == 0 &&
                           state->getCell(x0 - x, m * yv0 - 1)->getContent() ==
                               0) {
                  engine->addCommand(new MoveCommands(state, engine,
                                                      state->getMainCharacter(),
                                                      -x, m * yv0 - y0));
                  x0 = x0 - x, y0 = m * yv0 - 1;
                  break;
                }
              }
            }
          }
          while (!end && engine->getSize())
            ;
          xv0 = x0 / n, yv0 = y0 / m;
        }
        engine->addCommand(new MoveCommands(
            state, engine, state->getMainCharacter(), x1 - x0, y1 - y0));
        while (!end && engine->getSize())
          ;
        int xv = xv0 * n, yv = yv0 * m, i, j;
        if (state->getFight())
          for (auto chars : state->getFight()->getTeam(0)->getCharacters(
                   state->getFight()->getNb())) {
            do {
              i = xv + n / 6 + rand() % (2 * n / 3);
              j = yv + 2 * m / 3 + rand() % (m / 4);
            } while (state->getCell(i, j)->getContent() != nothing);
            engine->addCommand(
                new MoveCommand(state, chars, chars->getI(), chars->getJ()));
            engine->addCommand(new MoveCommand(state, chars, i, j));
            state->getCell(i, j)->setContent(perso);
          }
        while (!end && engine->getSize())
          ;
        engine->addCommand(new FightCommand(state, engine,
                                            state->getFight()->getTeam(0),
                                            state->getFight()->getTeam(1)));
      }

      if (state->two_ai && engine->getSize() == 0 && fight &&
          fight->getTurn() % 2 == 1 && fight->getTurn() != 0) {
        vector<Character*> vect =
            ai->getTurnOrder(fight->getFightingCharacters(0));
        for (auto c : vect) {
          if (state->two_ai)
            ai->run(c);
        }
        if (state->two_ai && fight->getFightingCharacters(0).size())
          engine->addCommand(new FightCommand(state, engine,
                                              state->getFight()->getTeam(0),
                                              state->getFight()->getTeam(1)));
      }

      if (engine->getSize() == 0 && fight && fight->getTurn() % 2 == 0 &&
          fight->getTurn() != 0) {
        vector<Character*> vect =
            ai->getTurnOrder(fight->getFightingCharacters(1));
        for (auto c : vect) {
          ai->run(c);
        }
        if (fight->getFightingCharacters(1).size())
          engine->addCommand(new FightCommand(state, engine,
                                              state->getFight()->getTeam(0),
                                              state->getFight()->getTeam(1)));
      }
    }
  });
  t1.join();
  t2.join();
  t3.join();
}

vector<Command*> NetworkClient::getServerCommands(Json::Value& out) {
  Http::Request request;
  request.setMethod(Http::Request::Get);
  request.setUri("/commands");
  request.setBody(to_string(actualcmd));

  Http::Response response = http.sendRequest(request);
  string output = response.getBody();
  cout << response.getStatus() << endl;
  Json::Reader reader;
  reader.parse(output, out);

  vector<Command*> commands;
  for (int i = 0; i < (int)out.size(); i++) {
    commands.push_back(Command::deserialize(out[i], state, engine));
  }
  actualcmd += out.size();
  return commands;
}

void NetworkClient::putServerCommand(Command* command) {
  Json::Value in;
  command->serialize(in);
  Json::StyledWriter writer;
  string input = writer.write(in);

  Http::Request request;
  request.setMethod(Http::Request::Put);
  request.setUri("/commands");
  request.setBody(input);

  Http::Response response = http.sendRequest(request);
  cout << response.getStatus() << endl;
}

int NetworkClient::getGameStatus() {
  Http::Request request;
  request.setMethod(Http::Request::Get);
  request.setUri("/game");

  Http::Response response = http.sendRequest(request);
  string output = response.getBody();
  cout << output << endl;
  Json::Reader reader;
  Json::Value out;
  reader.parse(output, out);
  int seed = out.get("seed", 0).asInt();
  srand(seed);
  return seed;
}

void NetworkClient::run() {
  State* state = new State();
  Engine* engine = new Engine();
  Render* render = new Render(state, engine);
  // AI* ai = new HeuristicAI(state, engine);
  state_init(state);
  putServerCommand(new FightCommand(state, engine, state->getTeams()[0],
                                    state->getTeams()[1]));
  putServerCommand(new MoveCommand(state, state->getMainCharacter(), 0, 0));
  Json::Value out;
  getServerCommands(out);
}