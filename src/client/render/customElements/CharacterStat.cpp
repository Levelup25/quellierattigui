#include "CharacterStat.h"

namespace render {
Element* CharacterStats(state::Character* pcharacter) {
  Rectangle container;

  container.recshape.setFillColor(sf::Color::White);
  container.recshape.setOutlineThickness(0);

  // get generator of icons
  SpriteGeneratorById* iconGenerator = SpriteGenerator::Icon::pdefault;

  // load icons with generator
  render::Sprite iconpv, iconpa, iconpm;
  iconpv.sprite = iconGenerator->getSprite(IconType::pv, {20, 20});
  iconpa.sprite = iconGenerator->getSprite(IconType::pa, {20, 20});
  iconpm.sprite = iconGenerator->getSprite(IconType::pm, {20, 20});

  if (pcharacter == nullptr) {
    pcharacter = new state::Character();
  }

  // Create lines
  Element *linepv, *linepa, *linepm;
  linepv = StatLine("pv", iconpv, pcharacter->getPvCurrent(),
                    pcharacter->getPvMax());
  linepa = StatLine("pa", iconpa, pcharacter->getPaCurrent(),
                    pcharacter->getPaMax());
  linepm = StatLine("pm", iconpm, pcharacter->getPmCurrent(),
                    pcharacter->getPmMax());

  // get line stat properties
  int lineWith = linepv->getSizeAbs().x;
  int lineHeight = linepv->getSizeAbs().y;

  // place lines
  int y = 0;
  linepv->setPosRelative({0, y});
  y += lineHeight;
  linepa->setPosRelative({0, y});
  y += lineHeight;
  linepm->setPosRelative({0, y});
  y += lineHeight;

  // add lines to parent
  container.add(linepv);
  container.add(linepa);
  container.add(linepm);

  // adjust parent size
  container.setSizeRelative({lineWith, y});

  return container.getCopy();
}

Element* StatLine(std::string statName,
                  render::Sprite statIcon,
                  int statValueCurrent,
                  int statValueMax) {
  // Assume that icon size is 20*20

  Element line;
  render::Text statNameText, statValues;

  // set size of sub element
  int nameWidth = 25, iconWidth = 20, valueWidth = 65;
  statNameText.setSizeRelative({nameWidth, iconWidth});
  statIcon.setSizeRelative({iconWidth, iconWidth});
  statValues.setSizeRelative({valueWidth, iconWidth});

  // set properties of element
  statNameText.setString(statName);
  statValues.setString(to_string(statValueCurrent) + "/" +
                       to_string(statValueMax));

  // place element
  int x = 0, sepx = 10;
  statNameText.setPosRelative({x, 0});
  x += nameWidth + sepx;
  statIcon.setPosRelative({x, 5});
  x += iconWidth + sepx;
  statValues.setPosRelative({x, 0});
  x += valueWidth;

  // add element to parent
  line.add(statNameText.getCopy());
  line.add(statIcon.getCopy());
  line.add(statValues.getCopy());

  // adjust parent size
  line.setSizeRelative({x, iconWidth + 10});

  return line.getCopy();
}

}  // namespace render
