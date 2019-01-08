#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
  cout << "Clic gauche : déplacement" << endl;
  cout << "Clic droit : choix personnage" << endl;
  cout << "Se déplacer au bord de l'écran change la vue sauf si un "
          "obstacle bloque"
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
  cout << "Appuyez sur R pour effectuer un rollback, puis de nouveau sur R "
          "pour revenir à l'état normal"
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

  vector<string> names = {"Goku", "Snake",  "Cloud", "Samus",  "Mario", "Lara",
                          "Link", "Kratos", "Dante", "Altair", "Ryu",   "Ash"};
  int n = state->getN(), m = state->getM();
  // int N = state->getI(), M = state->getJ();
  int xb = n * 1, yb = m * 1;
  // int xe = n * 1, ye = m * 1;
  int xe = xb, ye = yb;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
                          xb + rand() % (xe - xb + n),
                          yb + rand() % (ye - yb + m));
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
  // xb = n * 0, yb = m * 0;
  // xe = n * 2, ye = m * 2;
  // xe = xb, ye = yb;
  for (int i = 2; i < 11; i++) {
    xb = n * ((i - 1) % 3), yb = m * ((i - 1) / 3), xe = xb, ye = yb;
    for (int j = 0; j < 4; j++) {
      state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
                          xb + rand() % (xe - xb + n),
                          yb + rand() % (ye - yb + m));
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
  state->initialCharacters = state->getCharacters();
}

void launch_threads(State* state,
                    Render* render,
                    Engine* engine,
                    AI* ai,
                    bool two_ai) {
  bool end = false;
  thread t1([render, &end]() {
    render->display();
    end = true;
  });
  thread t2([engine, &end]() {
    sf::Clock clock;
    while (!end) {
      if (clock.getElapsedTime().asSeconds() >= 1.0 / 30) {
        engine->runCommand();
        clock.restart();
      }
    }
  });
  thread t3([ai, state, engine, &end, &two_ai]() {
    while (!end) {
      shared_ptr<Fight> fight = state->getFight();
      vector<Character*> maincharacters = state->getMainCharacters();
      if (two_ai && !fight && maincharacters.size() > 1 &&
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
                  engine->addCommand(
                      new MoveCommands(state, engine, maincharacters[0],
                                       n * (xv0 + 1) - 1 - x0, y));
                  x0 = n * (xv0 + 1), y0 = y0 + y;
                  break;
                } else if (state->getCell(n * (xv0 + 1) - 1, y0 - y)
                                   ->getContent() == 0 &&
                           state->getCell(n * (xv0 + 1), y0 - y)
                                   ->getContent() == 0) {
                  engine->addCommand(
                      new MoveCommands(state, engine, maincharacters[0],
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
                      state, engine, maincharacters[0], n * xv0 - x0, y));
                  x0 = n * xv0 - 1, y0 = y0 + y;
                  break;
                } else if (state->getCell(n * xv0, y0 - y)->getContent() == 0 &&
                           state->getCell(n * xv0 - 1, y0 - y)->getContent() ==
                               0) {
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[0], n * xv0 - x0, -y));
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
                  engine->addCommand(new MoveCommands(state, engine,
                                                      maincharacters[0], x,
                                                      m * (yv0 + 1) - 1 - y0));
                  x0 = x0 + x, y0 = m * (yv0 + 1);
                  break;
                } else if (state->getCell(x0 - x, m * (yv0 + 1) - 1)
                                   ->getContent() == 0 &&
                           state->getCell(x0 - x, m * (yv0 + 1))
                                   ->getContent() == 0) {
                  engine->addCommand(new MoveCommands(state, engine,
                                                      maincharacters[0], -x,
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
                      state, engine, maincharacters[0], x, m * yv0 - y0));
                  x0 = x0 + x, y0 = m * yv0 - 1;
                  break;
                } else if (state->getCell(x0 - x, m * yv0)->getContent() == 0 &&
                           state->getCell(x0 - x, m * yv0 - 1)->getContent() ==
                               0) {
                  engine->addCommand(new MoveCommands(
                      state, engine, maincharacters[0], -x, m * yv0 - y0));
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
        engine->addCommand(new MoveCommands(state, engine, maincharacters[0],
                                            x1 - x0, y1 - y0));
        while (!end && engine->getSize())
          ;
        int xv = xv0 * n, yv = yv0 * m, i, j;
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

      if (two_ai && engine->getSize() == 0 && fight &&
          fight->getTurn() % 2 == 1 && fight->getTurn() != 0) {
        vector<Character*> vect =
            ai->getTurnOrder(fight->getFightingCharacters(0));
        for (auto c : vect) {
          ai->run(c);
        }
        if (fight->getFightingCharacters(0).size())
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
    bool two_ai = false;

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
      two_ai = true;
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
      launch_threads(state, render, engine, ai, two_ai);
    }
  }
  return 0;
}
