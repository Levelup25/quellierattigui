#ifndef SHEETS
#define SHEETS 1

#include "../Sprites.h"
#include "CharacterStat.h"
#include "CustomElement.h"

namespace render {
Element* TeamSheet(state::Team* team = nullptr);
Element* CharacterSheet(state::Character* character = nullptr);
Element* WeaponSheet(state::Weapon* weapon = nullptr);
Element* AbilitySheet(state::Ability* ability = nullptr);
}  // namespace render

#endif
