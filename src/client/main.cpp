#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <thread>

#include "render/renderTest.h"

#include "engine.h"
#include "render.h"
#include "state.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;

#include "string.h"

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        // Livrable 1.1
        if (strcmp(argv[i], "hello") == 0) {
            cout << "Bonjour le monde !" << endl;
        }// Livrable 1.final
        else if (strcmp(argv[i], "state") == 0) {
            cout << "création d'un état" << endl;
            State* state = new State();

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
            Inventory* inv1 = goodGuys->getInventory();
            Inventory* inv2 = badGuys->getInventory();
            inv1->addItem(weapon1);
            inv2->addItem(weapon2);

            cout << "Équipement des armes" << endl;
            heros->setWeapon(weapon1);
            villain->setWeapon(weapon2);

            cout << "Lancement du combat" << endl;
            Fight* fight = new Fight(goodGuys, badGuys);
            state->setFight(fight);
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
            // state->delCharacter(3, 6);
            state->delCharacter(villain);

            cout << "Nombre de personnages actuels sur le terrain: "
                    << state->getCharacters().size() << endl;

            cout << "Fin du combat" << endl;

        }// Livrable 2.1
        else if (strcmp(argv[i], "render") == 0) {
            State* state = new State();
            for (int i = 0; i < 10; i++) {
                state->addCharacter(i, rand() % (12 * 4), (Direction) (rand() % 4), rand() % state->getI(), rand() % state->getJ());
                for (int j = 0; j < 1; j++) {
                    state->addCharacter(i, rand() % (12 * 4), (Direction) (rand() % 4), state->getI(), state->getJ());
                }
            }

            Render* render = new Render(state, new Engine);
            render->display();
        }// Livrable 2.2
        else if (strcmp(argv[i], "engine") == 0) {
            State* state = new State();

            state->addCharacter(0, rand() % (12 * 4), (Direction) (rand() % 4), 7, 0);
            state->addCharacter(1, rand() % (12 * 4), (Direction) (rand() % 4), 6, 4);
            for (int i = 0; i < 4; i++) {
                state->addCharacter(0, rand() % (12 * 4), (Direction) (rand() % 4));
                state->addCharacter(1, rand() % (12 * 4), (Direction) (rand() % 4));
            }

            Team* team1 = state->getTeams()[0];
            Team* team2 = state->getTeams()[1];

            Character* char1 = team1->getCharacter(0);
            Character* char2 = team2->getCharacter(0);

            Weapon* weapon1 = new Weapon();
            Weapon* weapon2 = new Weapon();

            Ability* ability1 = weapon1->getAbilities()[0];
            Ability* ability2 = weapon2->getAbilities()[0];

            ability1->setTarget(circle, 1, 5);
            ability2->setTarget(line, 1, 4);
            ability1->setEffect(circle, 0, 3);
            ability2->setEffect(line, 0, 2);

            Inventory* inv1 = team1->getInventory();
            Inventory* inv2 = team2->getInventory();

            inv1->addItem(weapon1);
            inv2->addItem(weapon2);

            char1->setWeapon(weapon1);
            char2->setWeapon(weapon2);

            Engine* engine = new Engine();
            Render* render = new Render(state, engine);

            cout << "Cliquez pour vous déplacer" << endl;
            cout << "Se déplacer au bord de l'écran " << endl;

            thread t1([engine]() {
                sf::Clock clock;
                while (1) {
                    if (clock.getElapsedTime().asSeconds() >= 0.1) {
                        engine->runCommand();
                                clock.restart();
                    }
                }
            });
            thread t2([render]() {
                render->display();
            });
            t1.join();
            t2.join();
        }// Livrable 2.final
        else if (strcmp(argv[i], "random_ai") == 0) {
        }// Livrable 3.1
        else if (strcmp(argv[i], "heuristic_ai") == 0) {
        }// Livrable 3.final
        else if (strcmp(argv[i], "rollback") == 0) {
        } else if (strcmp(argv[i], "deep_ai") == 0) {
        }// Livrable 4.1
        else if (strcmp(argv[i], "thread") == 0) {
        } else if (strcmp(argv[i], "record") == 0) {
        } else if (strcmp(argv[i], "play") == 0) {
        }// Livrables 4.2 et 4.final
        else if (strcmp(argv[i], "listen") == 0) {
        } else if (strcmp(argv[i], "network") == 0) {
        }
    }
    return 0;
}
