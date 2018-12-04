#ifndef SHEETS
#define SHEETS 1

#include "../Sprites.h"
#include "CharacterStat.h"
#include "CustomElement.h"

namespace render {
Element* CharacterSheet(state::Character* pcharacter = nullptr);
Element* WeaponSheet(state::Weapon* pweapon = nullptr);
}  // namespace render

#endif
