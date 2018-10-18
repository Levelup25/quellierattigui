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
        }// Livrable 1->final
        else if (strcmp(argv[i], "state") == 0) {
            cout << "création d'un état" << endl;
            State state;
            cout << "création de la carte du monde" << endl;
            World* world;
            world = state.getWorld();
            cout << "récupération et modification d'une cellule" << endl;
            Cell* cell = world->getCell(2, 2);
            cell->setElement(fire);
            world->setCell(2, 2, cell);
            cout << "le nouvel élément vaut " << (world->getCell(2, 2))->getElement() << endl;
            cout << "création de 2 personnages" << endl;
            Character* heros = new Character(1, 2, "Iron Man");
            Character* villain = new Character(3, 6, "Thanos");
            cout << "création de 2 équipes" << endl;
            //MainTeam* avengers = new MainTeam(heros, "Avengers");
            //Team* gentil = avengers;
            Team* gentil = new Team(heros, "Avengers");
            Team* mechant = new Team(villain, "Inconnu");
            cout << "ajout des personnages à la carte du monde" << endl;
            world->addCharacter(heros);
            world->addCharacter(villain);
            cout << "nombre de personnages actuels : " << world->getCharacters().size() << endl;
            cout << "création d'armes" << endl;
            Weapon* arme1 = new Weapon(fire, "armure MK32");
            Weapon* arme2 = new Weapon(earth, "gant de l'infini");
            cout << "ajout de capacités dans les armes" << endl;
            Ability* ability1 = new Ability("rayon repulseur", 3, 3, fire);
            Ability* ability2 = new Ability("gemme de l'esprit", 0, 2147483647, earth);
            arme1->addAbility(ability1);
            arme2->addAbility(ability2);
            cout << "placement des armes dans les inventaires" << endl;
            Item* item1 = arme1;
            Item* item2 = arme2;
            Inventory inv1 = gentil->getInventory();
            Inventory inv2 = mechant->getInventory();
            inv1.addItem(item1);
            inv2.addItem(item2);
            cout << "équipement des armes" << endl;
            heros->setWeapon(arme1);
            villain->setWeapon(arme2);
            cout << "lancement du combat" << endl;
            Fight* fight = new Fight(gentil, mechant);
            state.setFight(true);
            cout << "attaque du premier personnage" << endl;
            villain->getDamage(ability1);
            cout << "le second personnage a " << villain->getPv() << " pv" << endl;
            cout << "suppression du personnage" << endl;
            world->delCharacter(3, 6);
            cout << "nombre de personnages actuels : " << world->getCharacters().size() << endl;

        }// Livrable 2->1
        else if (strcmp(argv[i], "render") == 0) {
        }// Livrable 2->2
        else if (strcmp(argv[i], "engine") == 0) {
        }// Livrable 2->final
        else if (strcmp(argv[i], "random_ai") == 0) {
        }// Livrable 3->1
        else if (strcmp(argv[i], "heuristic_ai") == 0) {
        }// Livrable 3->final
        else if (strcmp(argv[i], "rollback") == 0) {
        } else if (strcmp(argv[i], "deep_ai") == 0) {
        }// Livrable 4->1
        else if (strcmp(argv[i], "thread") == 0) {
        } else if (strcmp(argv[i], "record") == 0) {
        } else if (strcmp(argv[i], "play") == 0) {
        }// Livrables 4->2 et 4->final
        else if (strcmp(argv[i], "listen") == 0) {
        } else if (strcmp(argv[i], "network") == 0) {
        }
    }
    return 0;
}
