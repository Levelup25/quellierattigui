#ifndef SPRITEGENERATOR
#define SPRITEGENERATOR

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "../../shared/state/ElementType.h"
#include "SpriteGeneratorById.h"

enum IconType { pv, pa, pm };

namespace SpriteGenerator {
extern void init();

namespace Tile {
extern render::SpriteGeneratorById *pdefault, *palt1, *palt2;
}

namespace Icon {
extern render::SpriteGeneratorById* pdefault;
}

}  // namespace SpriteGenerator

#endif