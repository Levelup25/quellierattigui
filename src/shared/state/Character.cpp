#include "Character.h"

namespace state {

    Character::Character (std::size_t i, std::size_t j, CharacterType type = human, int pv = 3, int pa = 3, int pm = 3) {
            this->type=type;
            this->pvMax=pv;
            this->paMax=pa;
            this->pmMax=pm;
    }

}

