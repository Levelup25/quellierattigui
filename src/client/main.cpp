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
  cout << "Entrée : passer le tour et actualiser pa et pm" << endl;
  cout << "Les informations sur le personnage actif sont affichés à gauche"
       << endl;
  cout << "Les informations sur la capacité sont affichés à droite" << endl;
  cout << "Pointer un personnage affiche également ses informations à droite"
       << endl;
  cout << "Appuyez sur C pour ouvrir/fermer la fiche du personnage" << endl;
  cout << "Appuyez sur W pour ouvrir/fermer la fiche d'arme" << endl;
  cout << "Cliquer sur un sprite dans une fenêtre peut ouvrir d'autres fenêtres"
       << endl;
}

void state_init(State* state) {
  vector<string> names = {"Goku", "Snake",  "Cloud", "Samus",  "Mario", "Lara",
                          "Link", "Kratos", "Dante", "Altair", "Ryu",   "Ash"};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      state->addCharacter(i, rand() % (12 * 4), (Direction)(rand() % 4),
                          12 + rand() % 12, 12 + rand() % 12);
      Character* c = state->getCharacters().back();
      c->setName(names[rand() % names.size()]);
      c->setPm(2 + rand() % 5);
      c->setPv(1 + rand() % 4);
      c->setPa(3 + rand() % 2);
      Weapon* w = new Weapon(1 + rand() % 18);
      c->setWeapon(w);
      for (auto a : w->getAbilities()) {
        int r3 = rand() % 3;
        for (int i = 0; i < r3; i++)
          a->addLv();
      }
    }
  }
}

void launch_threads(State* state, Render* render, Engine* engine, AI* ai) {
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
  thread t3([ai, state, engine, &end]() {
    while (!end) {
      if (engine->getSize() == 0 && state->isFighting() &&
          state->getFight()->getTurn() % 2 == 0) {
        vector<Character*> vect =
            ai->getTurnOrder(state->getFight()->getFightingCharacters(1));
        for (auto c : vect) {
          ai->run(c);
        }
        if (state->getFight()->getFightingCharacters(1).size() > 0)
          engine->addCommand(new FightCommand(state, nullptr, nullptr));
      }
    }
  });
  t1.join();
  t2.join();
  t3.join();
}

int main(int argc, char* argv[]) {
  srand(time(NULL));

  State* state = new State();
  state_init(state);
  Engine* engine = new Engine();
  Render* render = new Render(state, engine);
  AI* ai;

  for (int i = 1; i < argc; i++) {
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
      Ability* ability1 = new Ability("rayon repulseur", 3, 3, fire);
      Ability* ability2 =
          new Ability("gemme de l'esprit", 0, 2147483647, earth);

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
      ai = new HeuristicAI(state, engine);
    }  // Livrable 3.final
    else if (strcmp(argv[i], "rollback") == 0) {
    } else if (strcmp(argv[i], "deep_ai") == 0) {
      ai = new DeepAI(state, engine);
    }  // Livrable 4.1
    else if (strcmp(argv[i], "thread") == 0) {
    } else if (strcmp(argv[i], "record") == 0) {
    } else if (strcmp(argv[i], "play") == 0) {
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
