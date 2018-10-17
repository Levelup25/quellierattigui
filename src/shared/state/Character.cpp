#include "Character.h"

namespace state {

    Character::Character (std::size_t i, std::size_t j, std::string nom, CharacterType type, int pv, int pa, int pm) {
        this->i=i;
        this->j=j;
        this->nom=nom;
        this->type=type;
        this->pvMax=pv;
        this->paMax=pa;
        this->pmMax=pm;
    }

}

