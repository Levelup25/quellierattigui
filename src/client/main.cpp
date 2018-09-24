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
    
    for (int i=1;i<argc;i++) {
        // Livrable 1.1
        if (strcmp(argv[i],"hello")==0) { 
            cout << "Bonjour le monde !" << endl;
        }

        // Livrable 1.final
        if (strcmp(argv[i],"state")==0) { 

        }

        // Livrable 2.1
        if (strcmp(argv[i],"render")==0) { 

        }

        // Livrable 2.2
        if (strcmp(argv[i],"engine")==0) { 

        }

        // Livrable 2.final
        if (strcmp(argv[i],"random_ai")==0) { 

        }

        // Livrable 3.1
        if (strcmp(argv[i],"heuristic_ai")==0) { 

        }

        // Livrable 3.final
        if (strcmp(argv[i],"rollback")==0) { 

        }

        if (strcmp(argv[i],"deep_ai")==0) { 

        }

        // Livrable 4.1
        if (strcmp(argv[i],"thread")==0) { 

        }

        if (strcmp(argv[i],"record")==0) { 

        }

        if (strcmp(argv[i],"play")==0) { 

        }

        // Livrables 4.2 et 4.final
        if (strcmp(argv[i],"listen")==0) { 

        }

        if (strcmp(argv[i],"network")==0) { 

        }
    }
    return 0;
}
