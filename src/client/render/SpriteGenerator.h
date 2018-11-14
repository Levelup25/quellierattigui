#ifndef SPRITEGENERATOR
#define SPRITEGENERATOR

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "../../shared/state/ElementType.h"
#include "SpriteGeneratorById.h"

namespace SpriteGenerator {
extern void init();

namespace Tile {
extern render::SpriteGeneratorById *pdefault, *palt1, *palt2;
}
}  // namespace SpriteGenerator

#endif