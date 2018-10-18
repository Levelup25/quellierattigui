#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "state.h"

using namespace std;
using namespace state;

#include "string.h"

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    // Livrable 1->1
    if (strcmp(argv[i], "hello") == 0) {
      cout << "Bonjour le monde !" << endl;
    }  // Livrable 1->final
    else if (strcmp(argv[i], "state") == 0) {
      cout << "création d'un état" << endl;
      State state;

      cout << "Récupération de la carte du monde" << endl;
      World* world;
      world = state.getWorld();

      cout << "Récupération et modification d'une cellule" << endl;
      Cell* cell = world->getCell(2, 2);
      cell->setElement(fire);
      cout << "le nouvel élément vaut " << (world->getCell(2, 2))->getElement()
           << endl;

      cout << "Création de 2 personnages" << endl;
      Character* heros = new Character("Iron Man");
      Character* villain = new Character("Thanos");

      cout << "Création de 2 équipes" << endl;
      // MainTeam* avengers = new MainTeam(heros, "Avengers");
      // Team* goodGuys = avengers;
      Team* goodGuys = new Team(heros, "Avengers");
      Team* badGuys = new Team(villain, "Inconnu");

      cout << "Ajout des personnages à la carte du monde" << endl;
      world->addCharacter(heros, 1, 2);
      world->addCharacter(villain, 3, 6);

      cout << "Nombre de personnages actuels : "
           << world->getCharacters().size() << endl;

      cout << "Création d'armes" << endl;
      Weapon* arme1 = new Weapon(fire, "armure MK32");
      Weapon* arme2 = new Weapon(earth, "gant de l'infini");

      cout << "Création de capacités" << endl;
      Ability* ability1 = new Ability("rayon repulseur", 3, 3, fire);
      Ability* ability2 =
          new Ability("gemme de l'esprit", 0, 2147483647, earth);

      cout << "Ajout de capacités dans les armes" << endl;
      arme1->addAbility(ability1);
      arme2->addAbility(ability2);

      cout << "Placement des armes dans les inventaires" << endl;
      Inventory inv1 = goodGuys->getInventory();
      Inventory inv2 = badGuys->getInventory();
      inv1.addItem(arme1);
      inv2.addItem(arme2);

      cout << "Équipement des armes" << endl;
      heros->setWeapon(arme1);
      villain->setWeapon(arme2);

      cout << "Lancement du combat" << endl;
      Fight* fight = new Fight(goodGuys, badGuys);
      state.setFight(true);

      cout << "Le vilain peut infliger " << ability2->getDamage()
           << "dégat(s) avec sa capacité " << endl;
      cout << "Le second personnage a " << heros->getPv() << " pv" << endl;

      cout << "Suppression du personnage" << endl;
      world->delCharacter(3, 6);

      cout << "Nombre de personnages actuels sur le terrain: "
           << world->getCharacters().size() << endl;

      cout << "Fin du combat" << endl;
      state.setFight(false);

    }  // Livrable 2->1
    else if (strcmp(argv[i], "render") == 0) {
    }  // Livrable 2->2
    else if (strcmp(argv[i], "engine") == 0) {
    }  // Livrable 2->final
    else if (strcmp(argv[i], "random_ai") == 0) {
    }  // Livrable 3->1
    else if (strcmp(argv[i], "heuristic_ai") == 0) {
    }  // Livrable 3->final
    else if (strcmp(argv[i], "rollback") == 0) {
    } else if (strcmp(argv[i], "deep_ai") == 0) {
    }  // Livrable 4->1
    else if (strcmp(argv[i], "thread") == 0) {
    } else if (strcmp(argv[i], "record") == 0) {
    } else if (strcmp(argv[i], "play") == 0) {
    }  // Livrables 4->2 et 4->final
    else if (strcmp(argv[i], "listen") == 0) {
    } else if (strcmp(argv[i], "network") == 0) {
    }
  }
  return 0;
}
