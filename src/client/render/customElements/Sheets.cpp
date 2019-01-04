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
  Sprite* wpnSprite = new Sprite();
  if (character == nullptr)
    character = new state::Character();
  Element* charStat = CharacterStats(character);

  // config children
  CharacterName->setString(character->getName());
  Sprites* spriteGen = new Sprites();
  volatile auto id = character->getId();
  // cout << "id: " << id << endl;
  charSprite->sprite = spriteGen->getCharacterSprite(50, 50, id);
  charSprite->setSizeRelative({32, 32});
  wpnSprite->sprite =
      spriteGen->getWeaponSprite(50, 50, character->getWeapon()->getId());
  wpnSprite->setLink(WeaponSheet(character->getWeapon()));
  // charSprite->updateSizeFromTextureRect();
  wpnSprite->setSizeRelative({32, 32});

  // place children
  std::vector<Element*> children = {CharacterName, charSprite, wpnSprite,
                                    charStat};
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
        spriteGen->getAbilitySprite(50, 50, a->getName(), a->getLv());
    wpnAbilities[i]->setLink(AbilitySheet(weapon->getAbility(i)));
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

Element* AbilitySheet(state::Ability* ability) {
  Window abSheet;
  abSheet.ptitle->setString("");
  abSheet.pcontent->recshape.setFillColor(sf::Color::White);

  // create subelement/children
  Text* AbilityName = new Text();
  Sprite* abSprite = new Sprite();
  Sprite* targetSprite = new Sprite();
  Sprite* effectSprite = new Sprite();

  int size = 120;

  sf::RenderTexture targetTexture;
  targetTexture.create(size, size);
  int nb = 2 * ability->getTarget()[2] + 1;
  vector<vector<int>> targets = ability->getTargetZone({nb / 2, nb / 2});
  for (auto target : targets) {
    sf::RectangleShape rectangle(sf::Vector2f(size / nb, size / nb));
    rectangle.setPosition(
        sf::Vector2f(target[0] * size / nb, target[1] * size / nb));
    rectangle.setFillColor(sf::Color::Blue);
    targetTexture.draw(rectangle);
  }
  for (int i = 0; i <= size; i = i + size / nb) {
    sf::RectangleShape xline(sf::Vector2f(size, 1));
    xline.setPosition(sf::Vector2f(0, i));
    xline.setFillColor(sf::Color::Black);

    sf::RectangleShape yline(sf::Vector2f(1, size));
    yline.setPosition(sf::Vector2f(i, 0));
    yline.setFillColor(sf::Color::Black);

    targetTexture.draw(xline);
    targetTexture.draw(yline);
  }
  targetTexture.display();

  const sf::Texture& ttexture = targetTexture.getTexture();
  sf::Sprite tSprite(ttexture);
  targetSprite->sprite = tSprite;
  targetSprite->setSizeRelative({size, size});

  sf::RenderTexture effectTexture;
  effectTexture.create(size, size);
  effectTexture.clear();
  nb = 2 * ability->getEffect()[2] + 1;
  for (int i = 0; i <= size; i = i + size / nb) {
    sf::Vertex linex[] = {sf::Vertex(sf::Vector2f(0, i), sf::Color::Black),
                          sf::Vertex(sf::Vector2f(size, i), sf::Color::Black)};
    sf::Vertex liney[] = {sf::Vertex(sf::Vector2f(i, 0), sf::Color::Black),
                          sf::Vertex(sf::Vector2f(i, size), sf::Color::Black)};
    effectTexture.draw(linex, 2, sf::Lines);
    effectTexture.draw(liney, 2, sf::Lines);
  }
  effectTexture.display();
  const sf::Texture& etexture = effectTexture.getTexture();
  sf::Sprite eSprite(etexture);
  effectSprite->sprite = eSprite;
  effectSprite->setSizeRelative({size, size});

  // config children
  AbilityName->setString(ability->getName());
  Sprites* spriteGen = new Sprites();
  abSprite->sprite =
      spriteGen->getAbilitySprite(50, 50, ability->getName(), ability->getLv());
  // abSprite->updateSizeFromTextureRect();
  abSprite->setSizeRelative({50, 50});

  // place children
  std::vector<Element*> children = {AbilityName, abSprite, targetSprite,
                                    effectSprite};
  int spacepx = 20;
  int y = 0;
  for (size_t i = 0; i < children.size(); i++) {
    auto pchild = children[i];
    pchild->setPosRelative({"m", y});
    abSheet.pcontent->add(pchild);
    y += pchild->getSizeAbs().y;
    if (i != children.size() - 1)
      y += spacepx;
  }

  abSheet.pcontent->setSizeRelative({250, y});
  int winWidth, winHeight;
  winWidth = abSheet.pcontent->getSizeAbs().x;
  winHeight =
      abSheet.ptitleBar->getSizeAbs().y + abSheet.pcontent->getSizeAbs().y;
  abSheet.setSizeRelative({winWidth, winHeight});

  return abSheet.getCopy();
}

}  // namespace render
