#include <iostream>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"

using namespace std;
using namespace state;

#include "string.h"

int main(int argc,char* argv[]) 
{
    Exemple exemple;
    exemple.setX(53);
    
    testSFML();
    
    if (argc == 2) {
        // Livrable 1.1
        if (strcmp(argv[1],"hello")==0) { 
            cout << "Bonjour le monde !" << endl;
        }

        // Livrable 1.final
        if (strcmp(argv[1],"state")==0) { 

        }

        // Livrable 2.1
        if (strcmp(argv[1],"render")==0) { 

        }

        // Livrable 2.2
        if (strcmp(argv[1],"engine")==0) { 

        }

        // Livrable 2.final
        if (strcmp(argv[1],"random_ai")==0) { 

        }

        // Livrable 3.1
        if (strcmp(argv[1],"heuristic_ai")==0) { 

        }

        // Livrable 3.final
        if (strcmp(argv[1],"rollback")==0) { 

        }

        if (strcmp(argv[1],"deep_ai")==0) { 

        }

        // Livrable 4.1
        if (strcmp(argv[1],"thread")==0) { 

        }

        if (strcmp(argv[1],"record")==0) { 

        }

        if (strcmp(argv[1],"play")==0) { 

        }

        // Livrables 4.2 et 4.final
        if (strcmp(argv[1],"listen")==0) { 

        }

        if (strcmp(argv[1],"network")==0) { 

        }
    }
    return 0;
}
