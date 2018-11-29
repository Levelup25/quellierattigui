#ifndef CHARACTERSHEET
#define CHARACTERSHEET 1

#include "../Sprites.h"
#include "CharacterStat.h"
#include "CustomElement.h"

namespace render {
Element* CharacterSheet(state::Character* pcharacter = nullptr);
}

#endif
