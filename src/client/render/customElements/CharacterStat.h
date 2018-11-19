#ifndef CHARACTERSTATS
#define CHARACTERSTATS 1

#include "CustomElement.h"

namespace render {

Element* CharacterStats(state::Character* pcharacter = nullptr);
Element* StatLine(std::string statName,
                  render::Sprite statIcon,
                  int statValueCurrent,
                  int statValueMax);
}  // namespace render

#endif
