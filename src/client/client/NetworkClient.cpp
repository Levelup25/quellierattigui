#include "NetworkClient.h"
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "ai/HeuristicAI.h"

using namespace client;
using namespace engine;
using namespace state;
using namespace render;
using namespace ai;
using namespace std;
using namespace sf;

NetworkClient::NetworkClient(const string& url, int port) {
  this->url = url;
  this->port = port;
  this->http.setHost(url, port);
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

  // Engine loop and play sound
  thread t2([engine, state, &end, this]() {
    // Loads sounds - start
    cout << "Loading sounds..." << endl;
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
    cout << "Sounds loaded" << endl;
    // Load sounds - end

    // Engine loop
    Clock clk_engine, clk_sync_server;
    float period_engine = 1.0 / 30, period_sync_server = 5;
    deque<Command*> commands;
    while (!end) {
      while (engine->getSize()) {
        Command* command = engine->getCommand();
        if (command) {
          if (!command->getType().compare("MoveCommands") ||
              !command->getType().compare("AttackCommand")) {
            engine->runCommand();
          } else {
            putServerCommand(command);
            engine->clearCommand();
          }
        }
      }

      // Get commands for sync state - start
      if (clk_sync_server.getElapsedTime().asSeconds() >= period_sync_server) {
        deque<Command*> serverCommands = getServerCommands();
        for (auto cmd : serverCommands)
          commands.push_back(cmd);
        clk_sync_server.restart();
      }
      // Get commands for sync state - end

      // execute command from server - start
      if (!(clk_engine.getElapsedTime().asSeconds() >= period_engine)) {
        continue;
      }
      clk_engine.restart();

      if (commands.size() == 0) {
        continue;
      }

      Command* cmd = commands.front();
      string type = cmd->getType();
      Character* character = cmd->getCharacter();
      Team* team = nullptr;

      // Anylayse command type and play associated sound - start
      if (!type.compare("MoveCommand")) {
        int i =
            state->getCell(character->getI(), character->getJ())->getElement();
        if (move_sounds[i].getStatus() != 2) {
          // move_sounds[i].play();
        }
      }

      else if (!type.compare("AttackCommand")) {
        int i = character->getWeapon()
                    ->getAbility(
                        static_cast<AttackCommand*>(cmd)->getAbilityNumber())
                    ->getElement();
        if (attack_sounds[i].getStatus() != 2) {
          // attack_sounds[i].play();
        }
      }

      else if (!type.compare("FightCommand") && state->getFight()) {
        team = state->getFight()->getTeam(1);
      }
      // Anylayse command type and play associated sound - end

      cmd->execute();
      commands.pop_front();

      // play sound at end of fight - start
      if (!type.compare("FightCommand") && !state->getFight()) {
        vector<Team*> teams = state->getTeams();
        if (find(teams.begin(), teams.end(), team) != teams.end()) {
          // lose_sound.play();
        } else {
          // win_sound.play();
        }
      }
      // play sound at end of fight - end
    }
  });
  thread t3([ai, state, engine, &end, this]() {
    bool deploy = false;
    while (!end) {
      auto fight = state->getFight();
      if (!fight)
        deploy = false;
      if (fight && !fight->getTurn() && !deploy) {
        deploy = true;
        int i, j, nb = fight->getNb();
        int n = state->getN(), m = state->getM();
        int xv = ((int)state->getMainCharacter()->getI() / n) * n,
            yv = ((int)state->getMainCharacter()->getJ() / m) * m;

        for (auto oppchars : fight->getTeam(1)->getCharacters(nb)) {
          // if (!(oppchars->getI() >= xv + n / 6 &&
          //       oppchars->getI() < xv + n / 6 + 2 * n / 3 &&
          //       oppchars->getJ() >= yv + m / 12 &&
          //       oppchars->getJ() < yv + m / 12 + m / 4)) {
          do {
            i = xv + n / 6 + rand() % (2 * n / 3);
            j = yv + m / 12 + rand() % (m / 4);
          } while (state->getCell(i, j)->getContent() != nothing);
          engine->addCommand(new MoveCommand(state, oppchars, oppchars->getI(),
                                             oppchars->getJ()));
          engine->addCommand(new MoveCommand(state, oppchars, i, j));
          state->getCell(i, j)->setContent(perso);
        }
      }
      while (engine->getSize()) {
        Command* command = engine->getCommand();
        if (!command->getType().compare("MoveCommands") ||
            !command->getType().compare("AttackCommand")) {
          engine->runCommand();
        } else {
          putServerCommand(command);
          engine->clearCommand();
        }
      }
      if (!commands.size()) {
        deque<Command*> serverCommands = getServerCommands();
        for (auto cmd : serverCommands)
          commands.push_back(cmd);
      }
      if (commands.size() == 0 && fight && fight->getTurn() % 2 == 0 &&
          fight->getTurn() != 0) {
        vector<Character*> vect =
            ai->getTurnOrder(fight->getFightingCharacters(1));
        for (auto c : vect) {
          ai->run(c);
          vector<int> offset = {0, 0};
          deque<Command*> engineCommands = engine->getCommands();
          for (int i = 0; i < (int)engineCommands.size(); i++) {
            if (!engineCommands[i]->getType().compare("MoveCommands")) {
              offset[0] +=
                  static_cast<MoveCommands*>(engineCommands[i])->getDiff()[0];
              offset[1] +=
                  static_cast<MoveCommands*>(engineCommands[i])->getDiff()[1];
            } else if (!engineCommands[i]->getType().compare("AttackCommand")) {
              static_cast<AttackCommand*>(engineCommands[i])
                  ->offsetPosition(offset);
            }
          }
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

deque<Command*> NetworkClient::getServerCommands() {
  Http::Request request;
  request.setMethod(Http::Request::Get);
  request.setField("Content-Type", "application/x-www-form-urlencoded");
  request.setUri("/commands/" + to_string(idLastExecutedCmd));

  Http::Response response = this->http.sendRequest(request);
  string output = response.getBody();
  Json::Reader reader;
  Json::Value out;
  reader.parse(output, out);
  // cout << output << endl;

  deque<Command*> commands;
  if (out != Json::Value::null) {
    for (int i = 0; i < (int)out.size(); i++) {
      commands.push_back(Command::deserialize(out[i], state, engine));
    }
    idLastExecutedCmd += out.size();
  }
  return commands;
}

void NetworkClient::putServerCommand(Command* command) {
  if (!command)
    return;
  Json::Value in;
  command->serialize(in);
  Json::StyledWriter writer;
  string input = writer.write(in);

  Http::Request request;
  request.setMethod(Http::Request::Put);
  request.setField("Content-Type", "application/x-www-form-urlencoded");
  request.setUri("/commands");
  request.setBody(input);
  // cout << input << endl;

  Http::Response response = this->http.sendRequest(request);
}

int NetworkClient::getGameStatus() {
  Http::Request request;
  request.setMethod(Http::Request::Get);
  request.setField("Content-Type", "application/x-www-form-urlencoded");
  request.setUri("/game");

  Http::Response response = this->http.sendRequest(request);
  string output = response.getBody();
  if (response.getStatus() != sf::Http::Response::Status::Ok) {
    cout << "Erreur lors de la récupération de l'état originel du serveur"
         << endl;
    cout << "Statut: " << response.getStatus() << endl;
    cout << "body: " << output << endl;
    return 0;
  }
  Json::Reader reader;
  Json::Value out;
  reader.parse(output, out);
  int seed = out.get("seed", 0).asInt();
  return seed;
}

void NetworkClient::run() {
  cout << "Lancement du Jeu en mode multijoueur" << endl;
  Json::Reader reader;  // for parse response

  // check server co - start
  // Vérifie si on peut contacter le serveur (en récupérant la version)
  cout << "Connexion au serveur..." << endl;

  sf::Http::Request req_version;
  req_version.setMethod(sf::Http::Request::Get);
  // req_version.setField("Content-Type",
  // "application/x-www-form-urlencoded");
  req_version.setUri("/version");

  sf::Http::Response res_version = this->http.sendRequest(req_version);
  if (res_version.getStatus() != sf::Http::Response::Status::Ok) {
    cout << "Erreur lors de la connexion au serveur, veulliez essayer à "
            "nouveau plus tard"
         << endl;
    cout << "status: " << res_version.getStatus() << endl;
    cout << "body: " << res_version.getBody() << endl;
    return;
  }
  cout << "Connexion établie" << endl;

  // connexion avec pseudo à la partie - start
  unsigned int try_max = 10;
  unsigned int try_current = 0;
  bool connected = false;
  unsigned int id_player;
  unsigned int player_team_index;

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

    sf::Http::Response res_pseudo = this->http.sendRequest(req_pseudo);
    if (res_pseudo.getStatus() == sf::Http::Response::Status::Ok ||
        res_pseudo.getStatus() == sf::Http::Response::Status::Created) {
      connected = true;
      Json::Value res_pseudo_json;
      reader.parse(res_pseudo.getBody(), res_pseudo_json);
      id_player = res_pseudo_json.get("id", 0).asUInt();
      player_team_index = res_pseudo_json.get("teamId", 0).asUInt();
      cout << "Identification établie, id : " << id_player << endl;
    } else {
      connected = false;
      cout << "Identification échouée" << endl;
    }
  }

  // regenerate server state - start
  cout << "Récupération de l'état de la partie..." << endl;
  unsigned int seed = getGameStatus();
  this->state = new State(seed);
  this->state->mainTeamIndex = player_team_index;
  deque<Command*> server_commands = getServerCommands();
  for (auto ptr_cmd : server_commands) {
    ptr_cmd->execute();
  }
  cout << "Récupération de l'état de la partie terminée" << endl;
  // regenerate server state - end

  // launch game
  cout << "Lancement du jeu..." << endl;
  this->engine = new Engine();
  this->render = new Render(state, engine);
  this->ai = new HeuristicAI(state, engine);
  launch_threads(state, render, engine, ai);
  cout << "Jeu lancé" << endl;

  // Log out - start
  cout << "Deconnexion..." << endl;
  sf::Http::Request req_logout;
  req_logout.setMethod(sf::Http::Request::Post);
  req_logout.setUri("/players/" +
                    to_string(id_player));  // global var id_player
  req_logout.setField("Content-Type", "application/x-www-form-urlencoded");
  string data_str = "{\"isLogged\" : false}";
  req_logout.setBody(data_str);

  sf::Http::Response res_logout = this->http.sendRequest(req_logout);
  if (res_logout.getStatus() == sf::Http::Response::Status::Ok) {
    cout << "Deconnecté" << endl;
  } else {
    cout << "Erreur de déconnexion" << endl;
    cout << "status: " << res_logout.getStatus() << endl;
    cout << "body: " << res_logout.getBody() << endl;
  }
  // Log out - end
}
