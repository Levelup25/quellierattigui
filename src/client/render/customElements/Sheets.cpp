#include "Sheets.h"

namespace render {

Element* TeamSheet(state::Team* team) {
  Window teamSheet;
  teamSheet.ptitle->setString("");
  teamSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* TeamName = new Text();
  vector<Text*> charTexts;
  vector<Sprite*> charSprites;
  vector<Sprite*> wpnSprites;
  int i = team->getCharacters().size();
  while (i--) {
    charTexts.push_back(new Text());
    charSprites.push_back(new Sprite());
    wpnSprites.push_back(new Sprite());
  }

  // config children
  TeamName->setString(team->getName());
  Sprites* spriteGen = new Sprites();
  i = 0;
  for (auto character : team->getCharacters()) {
    charTexts[i]->setString(character->getName());
    charSprites[i]->sprite =
        spriteGen->getCharacterSprite(50, 50, character->getId());
    charSprites[i]->setLink(CharacterSheet(character));
    // charSprite->updateSizeFromTextureRect();
    charSprites[i]->setSizeRelative({32, 32});
    wpnSprites[i]->sprite =
        spriteGen->getWeaponSprite(50, 50, character->getWeapon()->getId());
    wpnSprites[i]->setLink(WeaponSheet(character->getWeapon()));
    // charSprite->updateSizeFromTextureRect();
    wpnSprites[i]->setSizeRelative({32, 32});
    i++;
  }

  // place children
  std::vector<Element*> children = {TeamName};
  for (int i = 0; i < (int)charTexts.size(); i++) {
    children.push_back(charTexts[i]);
    children.push_back(charSprites[i]);
    children.push_back(wpnSprites[i]);
  }
  int spacepx = 20;
  int y = 0;
  i = 0;

  auto pchild = children[i];
  pchild->setPosRelative({"m", y});
  teamSheet.pcontent->add(pchild);
  y += pchild->getSizeAbs().y;
  if (i != (int)children.size() - 1)
    y += spacepx;

  for (int i = 0; i < (int)children.size() / 3; i++) {
    auto pchild = children[3 * i + 1];
    pchild->setPosRelative({"m", y});
    teamSheet.pcontent->add(pchild);
    y += pchild->getSizeAbs().y;
    if (3 * i + 1 != (int)children.size() - 1)
      y += spacepx;

    pchild = children[3 * i + 2];
    pchild->setPosRelative({"25%", y});
    teamSheet.pcontent->add(pchild);
    // y += pchild->getSizeAbs().y;
    // if (3 * i + 2 != children.size() - 1)
    //   y += spacepx;

    pchild = children[3 * i + 3];
    pchild->setPosRelative({"75%", y});
    teamSheet.pcontent->add(pchild);
    y += pchild->getSizeAbs().y;
    if (3 * i + 3 != (int)children.size() - 1)
      y += spacepx;
  }

  teamSheet.pcontent->setSizeRelative({200, y + 50});
  int winWidth, winHeight;
  winWidth = teamSheet.pcontent->getSizeAbs().x;
  winHeight =
      teamSheet.ptitleBar->getSizeAbs().y + teamSheet.pcontent->getSizeAbs().y;
  teamSheet.setSizeRelative({winWidth, winHeight});

  return teamSheet.getCopy();
}

Element* CharacterSheet(state::Character* character) {
  Window charSheet;
  charSheet.ptitle->setString("");
  charSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* CharacterName = new Text();
  Sprite* charSprite = new Sprite();
  if (character == nullptr)
    character = new state::Character();
  Element* charStat = CharacterStats(character);

  // config children
  CharacterName->setString(character->getName());
  Sprites* spriteGen = new Sprites();
  volatile auto id = character->getId();
  // cout << "id: " << id << endl;
  charSprite->sprite = spriteGen->getCharacterSprite(50, 50, id);
  charSprite->setLink(WeaponSheet(character->getWeapon()));
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

Element* WeaponSheet(state::Weapon* weapon) {
  Window wpnSheet;
  wpnSheet.ptitle->setString("");
  wpnSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* WeaponName = new Text();
  Sprite* wpnSprite = new Sprite();
  vector<Sprite*> wpnAbilities;
  if (weapon == nullptr)
    weapon = new state::Weapon();
  // Element* wpnStat = WeaponStats(weapon);

  // config children
  WeaponName->setString(weapon->getName());
  Sprites* spriteGen = new Sprites();
  volatile auto id = weapon->getId();
  // cout << "id: " << id << endl;
  wpnSprite->sprite = spriteGen->getWeaponSprite(50, 50, id);
  // wpnSprite->updateSizeFromTextureRect();
  wpnSprite->setSizeRelative({32, 32});
  for (int i = 0; i < (int)weapon->getAbilities().size(); i++) {
    state::Ability* a = weapon->getAbility(i);
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

}  // namespace render
