#include "SpriteGenerator.h"

using namespace std;
using namespace render;
using namespace state;

render::SpriteGeneratorById* SpriteGenerator::Tile::pdefault = nullptr;
render::SpriteGeneratorById* SpriteGenerator::Tile::palt1 = nullptr;
render::SpriteGeneratorById* SpriteGenerator::Tile::palt2 = nullptr;

void SpriteGenerator::init() {
  std::map<int, std::string> mapIdFile;
  std::map<int, sf::IntRect> mapIdRect;
  string fileBegin = "res/tileset/isometric_pixel_flat_", fileEnd = ".png";

  sf::IntRect rect = {0, 0, 34, 24};
  mapIdRect[ElementType::neutral] = rect;
  mapIdRect[ElementType::water] = rect;
  mapIdRect[ElementType::earth] = rect;
  mapIdRect[ElementType::fire] = rect;
  mapIdRect[ElementType::wind] = rect;

  mapIdFile[ElementType::neutral] = fileBegin + "0000" + fileEnd;
  mapIdFile[ElementType::water] = fileBegin + "0028" + fileEnd;
  mapIdFile[ElementType::earth] = fileBegin + "0055" + fileEnd;
  mapIdFile[ElementType::fire] = fileBegin + "0037" + fileEnd;
  mapIdFile[ElementType::wind] = fileBegin + "0093" + fileEnd;
  SpriteGenerator::Tile::pdefault =
      new SpriteGeneratorById(mapIdFile, mapIdRect);

  mapIdFile[ElementType::neutral] = fileBegin + "0162" + fileEnd;
  mapIdFile[ElementType::water] = fileBegin + "0154" + fileEnd;
  mapIdFile[ElementType::earth] = fileBegin + "0156" + fileEnd;
  mapIdFile[ElementType::fire] = fileBegin + "0160" + fileEnd;
  mapIdFile[ElementType::wind] = fileBegin + "0153" + fileEnd;
  SpriteGenerator::Tile::palt1 = new SpriteGeneratorById(mapIdFile, mapIdRect);

  mapIdFile[ElementType::neutral] = fileBegin + "0226" + fileEnd;
  mapIdFile[ElementType::water] = fileBegin + "0222" + fileEnd;
  mapIdFile[ElementType::earth] = fileBegin + "0216" + fileEnd;
  mapIdFile[ElementType::fire] = fileBegin + "0213" + fileEnd;
  mapIdFile[ElementType::wind] = fileBegin + "0223" + fileEnd;
  SpriteGenerator::Tile::palt2 = new SpriteGeneratorById(mapIdFile, mapIdRect);
}