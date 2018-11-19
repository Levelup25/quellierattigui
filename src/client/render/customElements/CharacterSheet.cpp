#include "CharacterSheet.h"

namespace render {

Element* CharacterSheet() {
  Window charSheet;
  charSheet.ptitle->setString("");
  charSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* CharacterName = new Text();
  Sprite* charSprite = new Sprite();
  Element* charStat = CharacterStats();

  // config children
  CharacterName->setString("Dark knight");
  CharacterSprite* spriteGen = new CharacterSprite(32, 32);
  charSprite->sprite = spriteGen->getSprite();
  charSprite->updateSizeFromTextureRect();

  // place children
  std::vector<Element*> children = {CharacterName, charSprite, charStat};
  int spacepx = 20;
  int y = 0;
  for (size_t i = 0; i < children.size(); i++) {
    auto pchild = children[i];
    pchild->setPosRelative({"m", y});
    charSheet.pcontent->add(pchild);
    y += pchild->getSizeAbs().y;
    if (i != children.size() - 1)
      y += spacepx;
  }

  charSheet.pcontent->setSizeRelative({150, y});
  int winWidth, winHeight;
  winWidth = charSheet.pcontent->getSizeAbs().x;
  winHeight =
      charSheet.ptitleBar->getSizeAbs().y + charSheet.pcontent->getSizeAbs().y;
  charSheet.setSizeRelative({winWidth, winHeight});

  return charSheet.getCopy();
}

}  // namespace render
