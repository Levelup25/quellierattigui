#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "render/renderTest.h"

#include "state.h"

using namespace std;
using namespace state;

#include "string.h"

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; i++) {
    // Livrable 1->1
    if (strcmp(argv[i], "hello") == 0) {
      cout << "Bonjour le monde !" << endl;
    }

    // Livrable 1->final
    else if (strcmp(argv[i], "state") == 0) {
      cout << "création d'un état" << endl;
      State state;

      cout << "Récupération de la carte du monde" << endl;
      World* world;
      world = state.getWorld();

      cout << "Récupération et modification d'une cellule" << endl;
      cout << "l'élément vaut " << (world->getCell(2, 2))->getElement()
           << " et le contenu vaut " << (world->getCell(2, 2))->getContent()
           << endl;
      Cell* cell = world->getCell(2, 2);
      cell->setElement(earth);
      cell->setContent(house);
      cout << "le nouvel élément vaut " << (world->getCell(2, 2))->getElement()
           << " et le nouveau contenu " << (world->getCell(2, 2))->getContent()
           << endl;

      cout << "Création de 2 personnages" << endl;
      Character* heros = new Character("Iron Man", human, 3, 3, 3);
      Character* villain = new Character("Thanos", titan, 3, 3, 3);

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
      Inventory inv1 = goodGuys->getInventory();
      Inventory inv2 = badGuys->getInventory();
      inv1.addItem(weapon1);
      inv2.addItem(weapon2);

      cout << "Équipement des armes" << endl;
      heros->setWeapon(weapon1);
      villain->setWeapon(weapon2);

      cout << "Lancement du combat" << endl;
      Fight* fight = new Fight(goodGuys, badGuys);
      state.setFight(true);
      cout << "Le heros " << heros->getName() << " de l'équipe "
           << goodGuys->getName() << " possède " << heros->getPv() << "pv, "
           << heros->getPa() << "pa et " << heros->getPm() << "pm." << endl;
      cout << "Il peut attaquer avec " << weapon1->getName()
           << " qui possède la capacité " << ability1->getName()
           << " qui coute " << ability1->getPa() << "pa et inflige "
           << ability1->getDamage() << " dommages." << endl;

      cout << "Le villain " << villain->getName() << " de l'équipe "
           << badGuys->getName() << " possède " << villain->getPv() << "pv, "
           << villain->getPa() << "pa et " << villain->getPm() << "pm." << endl;
      cout << "Il peut attaquer avec " << weapon2->getName()
           << " qui possède la capacité " << ability2->getName()
           << " qui coute " << ability2->getPa() << "pa et inflige "
           << ability2->getDamage() << " dommages." << endl;

      cout << "Le heros attaque" << endl;
      villain->removePv(ability1->getDamage());
      cout << "Le second personnage a " << villain->getPv() << " pv et meurs"
           << endl;

      cout << "Suppression du personnage" << endl;
      world->delCharacter(3, 6);

      cout << "Nombre de personnages actuels sur le terrain: "
           << world->getCharacters().size() << endl;

      cout << "Fin du combat" << endl;
      state.setFight(false);

    }

    // Livrable 2->1
    else if (strcmp(argv[i], "render") == 0) {
      testRender();
    }

    // Livrable 2->2
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
