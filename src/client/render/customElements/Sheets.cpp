#include "Sheets.h"

namespace render {

Element* WeaponSheet(state::Weapon* pweapon) {
  Window wpnSheet;
  wpnSheet.ptitle->setString("");
  wpnSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* WeaponName = new Text();
  Sprite* wpnSprite = new Sprite();
  vector<Sprite*> wpnAbilities;
  if (pweapon == nullptr)
    pweapon = new state::Weapon();
  // Element* wpnStat = WeaponStats(pweapon);

  // config children
  WeaponName->setString(pweapon->getName());
  Sprites* spriteGen = new Sprites();
  volatile auto id = pweapon->getId();
  // cout << "id: " << id << endl;
  wpnSprite->sprite = spriteGen->getWeaponSprite(50, 50, id);
  // wpnSprite->updateSizeFromTextureRect();
  wpnSprite->setSizeRelative({32, 32});
  for (int i = 0; i < (int)pweapon->getAbilities().size(); i++) {
    state::Ability* a = pweapon->getAbility(i);
    wpnAbilities.push_back(new Sprite());
    wpnAbilities[i]->sprite =
        spriteGen->getAbilitySprite(50, 50, a->getElement(), a->getLv());
    wpnAbilities[i]->setSizeRelative({50, 50});
  }

  // place children
  std::vector<Element*> children = {WeaponName, wpnSprite};
  children.insert(children.end(), wpnAbilities.begin(), wpnAbilities.end());
  int spacepx = 20;
  int y = 0;
  for (size_t i = 0; i < children.size(); i++) {
    auto pchild = children[i];
    pchild->setPosRelative({"m", y});
    wpnSheet.pcontent->add(pchild);
    y += pchild->getSizeAbs().y;
    if (i != children.size() - 1)
      y += spacepx;
  }

  wpnSheet.pcontent->setSizeRelative({250, y});
  int winWidth, winHeight;
  winWidth = wpnSheet.pcontent->getSizeAbs().x;
  winHeight =
      wpnSheet.ptitleBar->getSizeAbs().y + wpnSheet.pcontent->getSizeAbs().y;
  wpnSheet.setSizeRelative({winWidth, winHeight});

  return wpnSheet.getCopy();
}

Element* CharacterSheet(state::Character* pcharacter) {
  Window charSheet;
  charSheet.ptitle->setString("");
  charSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* CharacterName = new Text();
  Sprite* charSprite = new Sprite();
  if (pcharacter == nullptr)
    pcharacter = new state::Character();
  Element* charStat = CharacterStats(pcharacter);

  // config children
  CharacterName->setString(pcharacter->getName());
  Sprites* spriteGen = new Sprites();
  volatile auto id = pcharacter->getId();
  // cout << "id: " << id << endl;
  charSprite->sprite = spriteGen->getCharacterSprite(50, 50, id);
  charSprite->setLink(WeaponSheet(pcharacter->getWeapon()));
  // charSprite->updateSizeFromTextureRect();
  charSprite->setSizeRelative({32, 32});

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
