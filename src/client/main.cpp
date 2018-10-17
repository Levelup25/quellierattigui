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
    // Livrable 1.1
    if (strcmp(argv[i], "hello") == 0) {
      cout << "Bonjour le monde !" << endl;
    }

    // Livrable 1.final
    else if (strcmp(argv[i], "state") == 0) {
        World world(10,10);
        Cell* cell=world.getCell(2,2);
        cell->setElement(fire);
        world.setCell(2,2,cell);
        cout<<(world.getCell(2,2))->getElement()<<endl;
        Character* c1=new Character(1,2);
        Character* c2=new Character(3,6);
        Character* c3=new Character(0,4);
        Character* c4=new Character(5,3);
        world.addCharacter(c1);
        world.addCharacter(c2);
        world.addCharacter(c3);
        world.addCharacter(c4);b
        cout<<world.getCharacters().size()<<endl;
        world.delCharacter(0,4);
        cout<<world.getCharacters().size()<<endl;
        world.delCharacter(1,5);
        cout<<world.getCharacters().size()<<endl;
        world.delCharacter(1,2);
        cout<<world.getCharacters().size()<<endl;
    }

    // Livrable 2.1
    else if (strcmp(argv[i], "render") == 0) {
    }

    // Livrable 2.2
    else if (strcmp(argv[i], "engine") == 0) {
    }

    // Livrable 2.final
    else if (strcmp(argv[i], "random_ai") == 0) {
    }

    // Livrable 3.1
    else if (strcmp(argv[i], "heuristic_ai") == 0) {
    }

    // Livrable 3.final
    else if (strcmp(argv[i], "rollback") == 0) {
    }

    else if (strcmp(argv[i], "deep_ai") == 0) {
    }

    // Livrable 4.1
    else if (strcmp(argv[i], "thread") == 0) {
    }

    else if (strcmp(argv[i], "record") == 0) {
    }

    else if (strcmp(argv[i], "play") == 0) {
    }

    // Livrables 4.2 et 4.final
    else if (strcmp(argv[i], "listen") == 0) {
    }

    else if (strcmp(argv[i], "network") == 0) {
    }
  }
  return 0;
}
