#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "string.h"

#include "render/renderTest.h"

#include "ai.h"
#include "client.h"
#include "engine.h"
#include "render.h"
#include "state.h"

using namespace std;
using namespace sf;
using namespace state;
using namespace render;
using namespace engine;
using namespace client;
using namespace ai;

void cout_terminal() {
  cout << "Roue des elements : " << endl;
  cout << " \033[1;34meau\033[0m   >  \033[1;31mfeu\033[0m" << endl;
  cout << "  ^ neutre v " << endl;
  cout << "\033[1;32mterre\033[0m  <  \033[1;33mair\033[0m" << endl;
  cout << "Attaque elementaire forte : dégats doublés et soins changés en "
          "degats faibles"
       << endl;
  cout << "Attaque elementaire faible : dégats divisés et soins divisés"
       << endl;
  cout << "Attaque elementaire miroir : dégats divisés et soins doublés"
       << endl;
  cout << "Les soins en surplus restent actifs pendant le tour courant" << endl;
  cout << endl;
  cout << "Clic gauche : déplacement" << endl;
  cout << "Clic droit : choix personnage" << endl;
  cout << "Se déplacer au bord de l'écran change la vue sauf si un obstacle "
          "bloque"
       << endl;
  cout << "Cliquez sur un personnage pour se battre. Dans ce cas : " << endl;
  cout << "Clic gauche sur la carte : déplacement" << endl;
  cout << "Clic gauche sur une capacité, puis sur la zone bleue : attaque"
       << endl;
  cout << "Clic droit : choix personnage ou annulation d'attaque" << endl;
  cout << "Entrée : finir le déploiement/passer le tour et actualiser pa et pm"
       << endl;
  cout << "Les informations sur le personnage actif sont affichés à gauche"
       << endl;
  cout << "Les informations sur la capacité sont affichés à droite" << endl;
  cout << "Pointer un personnage affiche également ses informations à droite"
       << endl;
  cout << "Appuyez sur T pour ouvrir/fermer la fiche d'équipe (long à générer)"
       << endl;
  cout << "Appuyez sur C pour ouvrir/fermer la fiche du personnage" << endl;
  cout << "Appuyez sur W pour ouvrir/fermer la fiche d'arme" << endl;
  cout << "Appuyez sur A pour ouvrir/fermer la fiche des compétences d'armes"
       << endl;
  cout << "Cliquer sur un sprite dans une fenêtre peut ouvrir d'autres fenêtres"
       << endl;
  cout << "Appuyez sur R pour activer/désactiver le rollback" << endl;
  cout << "Appuyez sur I pour activer/désactiver l'intelligence artificielle"
       << endl;
  cout << "Appuyez sur M pour activer/désactiver la mini carte" << endl;
  cout << "Le jeu possède des effets sonores donc pensez à allumer le son!"
       << endl;
}

void state_init(State* state) {
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

void launch_threads(State* state, Render* render, Engine* engine, AI* ai) {
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

  // Engine loop and play sound
  thread t2([engine, state, &end]() {
    // Loads sounds - start
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
    // Load sounds - end

    Clock clock;
    while (!end) {
      if (!(clock.getElapsedTime().asSeconds() >= 1.0 / 30)) {
        continue;
      }
      clock.restart();

      Command* cmd = engine->getCommand();
      if (!cmd) {
        continue;
      }
      string type = cmd->getType();
      Character* character = cmd->getCharacter();
      Team* team = nullptr;

      // Anylayse command type and play associated sound - start
      if (!type.compare("MoveCommand")) {
        int i =
            state->getCell(character->getI(), character->getJ())->getElement();
        if (move_sounds[i].getStatus() != 2) {
          move_sounds[i].play();
        }
      }

      else if (!type.compare("AttackCommand")) {
        int i = character->getWeapon()
                    ->getAbility(
                        static_cast<AttackCommand*>(cmd)->getAbilityNumber())
                    ->getElement();
        if (attack_sounds[i].getStatus() != 2) {
          attack_sounds[i].play();
        }
      }

      else if (!type.compare("FightCommand") && state->getFight()) {
        team = state->getFight()->getTeam(1);
      }
      // Anylayse command type and play associated sound - end

      engine->runCommand();

      // play sound at end of fight - start
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
      // play sound at end of fight - end
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

State getStateFromServer() {
  cout << "Chargement de l'état de la partie..." << endl;
  // TODO
  cout << "Chargement de l'état de la parite terminé" << endl;
}

// todo : rename
bool launch_client_network() {
  cout << "Lancement du Jeu en mode multijoueur" << endl;

  // Vérifie si on peut contacter le serveur (en récupérant la version)
  cout << "Connexion au serveur..." << endl;
  sf::Http http_manager;
  http_manager.setHost("localhost", 8080);

  sf::Http::Request req_version;
  req_version.setMethod(sf::Http::Request::Get);
  req_version.setUri("/version");

  sf::Http::Response res_version = http_manager.sendRequest(req_version);
  if (res_version.getStatus() != sf::Http::Response::Status::Ok) {
    cout << "Erreur lors de la connexion au serveur, veulliez essayer à "
            "nouveau plus tard"
         << endl;
    cout << "status: " << res_version.getStatus() << endl;
    cout << "body: " << res_version.getBody() << endl;
    return false;
  }
  cout << "Connexion établie" << endl;

  // connexion avec pseudo à la partie
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

    sf::Http::Response res_pseudo = http_manager.sendRequest(req_pseudo);
    if (res_pseudo.getStatus() == sf::Http::Response::Status::Ok ||
        res_pseudo.getStatus() == sf::Http::Response::Status::Created) {
      connected = true;
      cout << "Identification établie" << endl;
    } else {
      connected = false;
      cout << "Identification échouée" << endl;
    }
    cout << "status: " << res_pseudo.getStatus() << endl;
    cout << "body: " << res_pseudo.getBody() << endl;
  }

  State state = getStateFromServer();

  return true;
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    ifstream file;
    Json::Reader reader;
    Json::Value root;
    unsigned int seed = time(NULL);
    if (strcmp(argv[i], "play") == 0) {
      file.open("replay.txt");
      reader.parse(file, root);
      seed = root[0].get("seed", 0).asUInt();
    }

    srand(seed);
    State* state = new State();
    state->seed = seed;
    state_init(state);
    Engine* engine = new Engine();
    Render* render = new Render(state, engine);
    AI* ai = new HeuristicAI(state, engine);

    // Livrable 1.1
    if (strcmp(argv[i], "hello") == 0) {
      cout << "Bonjour le monde !" << endl;
    }  // Livrable 1.final
    else if (strcmp(argv[i], "state") == 0) {
      cout << "création d'un état" << endl;
      state = new State();

      cout << "Récupération et modification d'une cellule" << endl;
      cout << "l'élément vaut " << (state->getCell(2, 2))->getElement()
           << " et le contenu vaut " << (state->getCell(2, 2))->getContent()
           << endl;
      Cell* cell = state->getCell(2, 2);
      cell->setElement(earth);
      cell->setContent(tree);
      cout << "le nouvel élément vaut " << (state->getCell(2, 2))->getElement()
           << " et le nouveau contenu " << (state->getCell(2, 2))->getContent()
           << endl;

      cout << "Création de 2 personnages" << endl;
      Character* heros = new Character(0, "Iron Man", human, 3, 3, 3);
      Character* villain = new Character(0, "Thanos", titan, 3, 3, 3);

      cout << "Création de 2 équipes" << endl;
      Team* goodGuys = new Team("Avengers");
      Team* badGuys = new Team("Inconnu");
      state->addTeam(goodGuys);
      state->addTeam(badGuys);

      cout << "Ajout des personnages à la carte du monde" << endl;
      state->addCharacter(heros, goodGuys, 1, 2);
      state->addCharacter(villain, badGuys, 3, 6);

      cout << "Nombre de personnages actuels : "
           << state->getCharacters().size() << endl;

      cout << "Création de 2 armes" << endl;
      Weapon* weapon1 = new Weapon(fire, "armure MK32");
      Weapon* weapon2 = new Weapon(earth, "gant de l'infini");

      cout << "Création de 2 capacités" << endl;
      Ability* ability1 = new Ability(3, 3, fire, "rayon repulseur");
      Ability* ability2 =
          new Ability(0, 2147483647, earth, "gemme de l'esprit");

      cout << "Ajout de capacités dans les armes" << endl;
      weapon1->addAbility(ability1);
      weapon2->addAbility(ability2);

      cout << "Placement des armes dans les inventaires" << endl;
      state::Inventory* inv1 = goodGuys->getInventory();
      state::Inventory* inv2 = badGuys->getInventory();
      inv1->addItem(weapon1);
      inv2->addItem(weapon2);

      cout << "Équipement des armes" << endl;
      heros->setWeapon(weapon1);
      villain->setWeapon(weapon2);

      cout << "Lancement du combat" << endl;
      state->setFight((shared_ptr<Fight>)new Fight(goodGuys, badGuys));
      cout << "Le heros " << heros->getName() << " de l'équipe "
           << goodGuys->getName() << " possède " << heros->getPvCurrent()
           << "pv, " << heros->getPaCurrent() << "pa et "
           << heros->getPmCurrent() << "pm." << endl;
      cout << "Il peut attaquer avec " << weapon1->getName()
           << " qui possède la capacité " << ability1->getName()
           << " qui coute " << ability1->getPa() << "pa et inflige "
           << ability1->getDamage() << " dommages." << endl;

      cout << "Le villain " << villain->getName() << " de l'équipe "
           << badGuys->getName() << " possède " << villain->getPvCurrent()
           << "pv, " << villain->getPaCurrent() << "pa et "
           << villain->getPmCurrent() << "pm." << endl;
      cout << "Il peut attaquer avec " << weapon2->getName()
           << " qui possède la capacité " << ability2->getName()
           << " qui coute " << ability2->getPa() << "pa et inflige "
           << ability2->getDamage() << " dommages." << endl;

      cout << "Le heros attaque" << endl;
      villain->removePv(ability1->getDamage());
      cout << "Le second personnage a " << villain->getPvCurrent()
           << " pv et meurs" << endl;

      cout << "Suppression du personnage" << endl;
      // state->delCharacter(3, 6);
      state->delCharacter(villain);

      cout << "Nombre de personnages actuels sur le terrain: "
           << state->getCharacters().size() << endl;

      cout << "Fin du combat" << endl;

    }  // Livrable 2.1
    else if (strcmp(argv[i], "render") == 0) {
      render->display();
    }  // Livrable 2.2
    else if (strcmp(argv[i], "renderTest") == 0) {
      testRender();
    } else if (strcmp(argv[i], "engine") == 0) {
      ai = new AI();
    }  // Livrable 2.final
    else if (strcmp(argv[i], "random_ai") == 0) {
      ai = new RandomAI(state, engine);
    }  // Livrable 3.1
    else if (strcmp(argv[i], "heuristic_ai") == 0) {
      // ai = new HeuristicAI(state, engine);
    }  // Livrable 3.final
    else if (strcmp(argv[i], "rollback") == 0) {
    } else if (strcmp(argv[i], "deep_ai") == 0) {
      ai = new DeepAI(state, engine);
    }  // Livrable 4.1
    else if (strcmp(argv[i], "thread") == 0) {
      state->two_ai = true;
    } else if (strcmp(argv[i], "record") == 0) {
    } else if (strcmp(argv[i], "play") == 0) {
      int size = root.size();
      for (int k = 1; k < size; k++) {
        engine->addCommand(Command::deserialize(root[k], state, engine));
      }
      file.close();

      ofstream del;
      del.open("replay.txt", ios::trunc);
      del.close();
      // srand(time(NULL));
    }  // Livrables 4.2 et 4.final
    // else if (strcmp(argv[i], "listen") == 0) {
    // }
    else if (strcmp(argv[i], "network") == 0) {
      // todo : merge and clean network client launcher
      if (!launch_client_network())
        return 1;

      cout_terminal();
      NetworkClient client("localhost", 8080);
      client.run();
    }

    if (strcmp(argv[i], "hello") && strcmp(argv[i], "state") &&
        strcmp(argv[i], "render") && strcmp(argv[i], "renderTest") &&
        strcmp(argv[i], "network")) {
      cout_terminal();
      launch_threads(state, render, engine, ai);
    }
  }
  return 0;
}
