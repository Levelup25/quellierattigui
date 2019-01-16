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
  idLastExecutedCmd = 0;
}

void NetworkClient::launch_threads(State* state,
                                   Render* render,
                                   Engine* engine,
                                   AI* ai) {
  bool end = false;

  // Render
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

  // Engine
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

    sounds = {"neutral", "water", "rock", "fire", "wind"};
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

  // Ai
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
  request.setField("Content-Type", "application/x-www-form-urlencoded");
  request.setUri("/commands/" + to_string(idLastExecutedCmd));

  Http::Response response = http.sendRequest(request);
  string output = response.getBody();
  cout << output << endl;
  Json::Reader reader;
  reader.parse(output, out);

  vector<Command*> commands;
  for (int i = 0; i < (int)out.size(); i++) {
    commands.push_back(Command::deserialize(out[i], state, engine));
  }
  idLastExecutedCmd += out.size();
  return commands;
}

void NetworkClient::putServerCommand(Command* command) {
  Json::Value in;
  command->serialize(in);
  Json::StyledWriter writer;
  string input = writer.write(in);

  Http::Request request;
  request.setMethod(Http::Request::Put);
  request.setField("Content-Type", "application/x-www-form-urlencoded");
  request.setUri("/commands");
  request.setBody(input);

  Http::Response response = http.sendRequest(request);
}

int NetworkClient::getGameStatus() {
  Http::Request request;
  request.setMethod(Http::Request::Get);
  request.setField("Content-Type", "application/x-www-form-urlencoded");
  request.setUri("/game");

  Http::Response response = http.sendRequest(request);
  string output = response.getBody();
  cout << output << endl;
  Json::Reader reader;
  Json::Value out;
  reader.parse(output, out);
  int seed = out.get("seed", 0).asInt();
  return seed;
}

void NetworkClient::run() {
  cout << "Lancement du Jeu en mode multijoueur" << endl;

  // check server co - start
  // Vérifie si on peut contacter le serveur (en récupérant la version)
  cout << "Connexion au serveur..." << endl;

  sf::Http::Request req_version;
  req_version.setMethod(sf::Http::Request::Get);
  // req_version.setField("Content-Type", "application/x-www-form-urlencoded");
  req_version.setUri("/version");

  sf::Http::Response res_version = http.sendRequest(req_version);
  if (res_version.getStatus() != sf::Http::Response::Status::Ok) {
    cout << "Erreur lors de la connexion au serveur, veulliez essayer à "
            "nouveau plus tard"
         << endl;
    cout << "status: " << res_version.getStatus() << endl;
    cout << "body: " << res_version.getBody() << endl;
    return;
  }
  cout << "Connexion établie" << endl;
  // check server co - end

  // connexion avec pseudo à la partie - start
  unsigned int try_max = 10;
  unsigned int try_current = 0;
  bool connected = false;
  string pseudo;
  while (!connected && try_current < try_max) {
    cout << "Veuillez rentrez votre pseudo : ";
    pseudo = "";
    cin >> pseudo;
    cout << "Identification en  cours..." << endl;

    sf::Http::Request req_pseudo;
    req_pseudo.setMethod(sf::Http::Request::Put);
    req_pseudo.setUri("/players");
    req_pseudo.setField("Content-Type", "application/x-www-form-urlencoded");
    string data_str = "{\"pseudo\" : \"" + pseudo + "\"}";
    req_pseudo.setBody(data_str);

    sf::Http::Response res_pseudo = http.sendRequest(req_pseudo);
    if (res_pseudo.getStatus() == sf::Http::Response::Status::Ok ||
        res_pseudo.getStatus() == sf::Http::Response::Status::Created) {
      connected = true;
      cout << "Identification établie" << endl;
    } else {
      connected = false;
      cout << "Identification échouée" << endl;
    }
    // cout << "status: " << res_pseudo.getStatus() << endl;
    // cout << "body: " << res_pseudo.getBody() << endl;
  }
  if (!connected) {
    return;
  }
  // connexion avec pseudo à la partie - end

  // récupération seed état du serveur - start
  cout << "Récupération de l'état de la partie..." << endl;
  sf::Http::Request req_seed;
  req_seed.setMethod(sf::Http::Request::Get);
  req_seed.setUri("/game");
  req_seed.setField("Content-Type", "application/x-www-form-urlencoded");

  sf::Http::Response res_seed = http.sendRequest(req_seed);
  string res_seed_body = res_seed.getBody();
  if (res_seed.getStatus() != sf::Http::Response::Status::Ok) {
    cout << "Erreur lors de la récupération de l'état originel du serveur"
         << endl;
    cout << "Statut: " << res_seed.getStatus() << endl;
    cout << "body: " << res_seed_body << endl;
    return;
  }
  Json::Reader reader;
  Json::Value res_seed_json;
  reader.parse(res_seed_body, res_seed_json);
  unsigned int seed = res_seed_json.get("seed", 0).asUInt();
  // récupération seed état du serveur - end

  // regenerate server state - start
  State* state = new State(seed);
  Json::Value server_commands_json;
  vector<Command*> server_commands = getServerCommands(server_commands_json);
  for (auto ptr_cmd : server_commands) {
    ptr_cmd->execute();
  }
  cout << "Récupération de l'état de la partie terminée" << endl;
  // regenerate server state - end

  // launch game
  cout << "Lancement du jeu..." << endl;

  // Render* render = new Render(state, engine);
  // AI* ai = new HeuristicAI(state, engine);

  // putServerCommand(new FightCommand(state, engine, state->getTeams()[0],
  //                                   state->getTeams()[1]));
  // putServerCommand(new MoveCommand(state, state->getMainCharacter(), 0, 0));
  // putServerCommand(new MoveCommand(state, state->getMainCharacter(), 1, 1));
  // out = Json::Value::null;
  // getServerCommands(out);
  // putServerCommand(new MoveCommand(state, state->getMainCharacter(), 2, 2));
  // getServerCommands(out);
  // getServerCommands(out);

  // cout << "Jeux lancé" << endl;
}