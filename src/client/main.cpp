#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include "string.h"

#include "render/renderTest.h"

#include "ai.h"
#include "engine.h"
#include "render.h"
#include "state.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
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
  vector<string> names = {"Goku",   "Snake",  "Cloud",    "Samus",
                          "Mario",  "Lara",   "Link",     "Kratos",
                          "Dante",  "Altair", "Ryu",      "Sacha",
                          "Jotaro", "Yugi",   "Meliodas", "Kirito"};
  int n = state->getN(), m = state->getM();
  int N = state->getI(), M = state->getJ();
  int xb = N / 4, yb = M / 4;
  int xe = 3 * N / 4, ye = 3 * M / 4;
  // int xe = xb, ye = yb;
  for (int i = 0; i < 1; i++) {
    // for (int j = 0; j < 4; j++) {
    //   state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
    //                       xb + rand() % (xe - xb), yb + rand() % (ye - yb));
    //   Character* c = state->getCharacters().back();
    //   c->setName(names[rand() % names.size()]);
    //   c->setPm(2 + rand() % 5);
    //   c->setPv(1 + rand() % 4);
    //   c->setPa(3 + rand() % 2);
    //   Weapon* w = new Weapon(rand() % nb);
    //   c->setWeapon(w);
    //   for (auto a : w->getAbilities()) {
    //     int r3 = rand() % 3;
    //     for (int i = 0; i < r3; i++)
    //       a->addLv();
    //   }
    // }
  }
  // xb = n * 0, yb = m * 0;
  // xe = n * 2, ye = m * 2;
  // xe = xb, ye = yb;
  for (int i = 0; i < nbteams; i++) {
    // xb = n * ((i - 1) % 3), yb = m * ((i - 1) / 3), xe = xb, ye = yb;
    for (int j = 0; j < 3; j++) {
      state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
                          rand() % N, rand() % M);
      Character* c = state->getCharacters().back();
      c->setName(names[rand() % names.size()]);
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
    xb = 3 * (i % 2) * N / 4, yb = 3 * (i / 2) * M / 4, xe = xb + N / 4,
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

  int xv = ((int)state->getMainCharacter()->getI() / n) * n,
      yv = ((int)state->getMainCharacter()->getJ() / m) * m;
  for (int i = nbteams + 4; i <= nbteams + 7; i++) {
    for (int j = 0; j < 4; j++) {
      state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
                          xv + rand() % n, yv + rand() % m);
      Character* c = state->getCharacters().back();
      c->setName(names[rand() % names.size()]);
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
  state->mainTeamIndex = state->getTeams().size() - 1;
  // state->initialCharacters = state->getCharacters();
}

void launch_threads(State* state, Render* render, Engine* engine, AI* ai) {
  bool end = false;
  thread t1([render, &end]() {
    sf::Music music;
    music.openFromFile("res/theme.wav");
    music.play();
    music.setLoop(true);
    music.setVolume(50.f);
    render->display();
    music.stop();
    end = true;
  });
  thread t2([engine, &end]() {
    sf::SoundBuffer move_buffer;
    move_buffer.loadFromFile("res/sounds/moving.ogg");
    sf::Sound move_sound;
    move_sound.setBuffer(move_buffer);

    sf::SoundBuffer attack_buffer;
    attack_buffer.loadFromFile("res/sounds/explode.ogg");
    sf::Sound attack_sound;
    attack_sound.setBuffer(attack_buffer);

    sf::SoundBuffer fight_buffer;
    fight_buffer.loadFromFile("res/sounds/cheer.ogg");
    sf::Sound fight_sound;
    fight_sound.setBuffer(fight_buffer);

    sf::Clock clock;
    while (!end) {
      if (clock.getElapsedTime().asSeconds() >= 1.0 / 30) {
        clock.restart();
        string type = engine->getCommand()->getType();
        engine->runCommand();
        if (!type.compare("MoveCommand") && move_sound.getStatus() != 2) {
          attack_sound.stop();
          fight_sound.stop();
          move_sound.play();
        } else if (!type.compare("AttackCommand") &&
                   attack_sound.getStatus() != 2) {
          move_sound.stop();
          fight_sound.stop();
          attack_sound.play();
        } else if (!type.compare("FightCommand") &&
                   attack_sound.getStatus() != 2) {
          move_sound.stop();
          attack_sound.stop();
          fight_sound.play();
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
        int min = abs(maincharacters[1]->getI() - x0) +
                  abs(maincharacters[1]->getJ() - y0),
            imin = 1;
        for (int i = 2; i < (int)maincharacters.size(); i++) {
          int dist = abs(maincharacters[i]->getI() - x0) +
                     abs(maincharacters[i]->getJ() - y0);
          if (dist < min) {
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
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex],
                      n * (xv0 + 1) - 1 - x0, y));
                  x0 = n * (xv0 + 1), y0 = y0 + y;
                  break;
                } else if (state->getCell(n * (xv0 + 1) - 1, y0 - y)
                                   ->getContent() == 0 &&
                           state->getCell(n * (xv0 + 1), y0 - y)
                                   ->getContent() == 0) {
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex],
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
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex],
                      n * xv0 - x0, y));
                  x0 = n * xv0 - 1, y0 = y0 + y;
                  break;
                } else if (state->getCell(n * xv0, y0 - y)->getContent() == 0 &&
                           state->getCell(n * xv0 - 1, y0 - y)->getContent() ==
                               0) {
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex],
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
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex], x,
                      m * (yv0 + 1) - 1 - y0));
                  x0 = x0 + x, y0 = m * (yv0 + 1);
                  break;
                } else if (state->getCell(x0 - x, m * (yv0 + 1) - 1)
                                   ->getContent() == 0 &&
                           state->getCell(x0 - x, m * (yv0 + 1))
                                   ->getContent() == 0) {
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex], -x,
                      m * (yv0 + 1) - 1 - y0));
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
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex], x,
                      m * yv0 - y0));
                  x0 = x0 + x, y0 = m * yv0 - 1;
                  break;
                } else if (state->getCell(x0 - x, m * yv0)->getContent() == 0 &&
                           state->getCell(x0 - x, m * yv0 - 1)->getContent() ==
                               0) {
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[state->mainTeamIndex], -x,
                      m * yv0 - y0));
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
            state, engine, maincharacters[state->mainTeamIndex], x1 - x0,
            y1 - y0));
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
    else if (strcmp(argv[i], "listen") == 0) {
    } else if (strcmp(argv[i], "network") == 0) {
    }

    if (strcmp(argv[i], "hello") != 0 && strcmp(argv[i], "state") != 0 &&
        strcmp(argv[i], "render") != 0 && strcmp(argv[i], "renderTest") != 0) {
      cout_terminal();
      launch_threads(state, render, engine, ai);
    }
  }
  return 0;
}
