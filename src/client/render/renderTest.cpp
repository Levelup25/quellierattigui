#include "renderTest.h"

void testRender() {
  SpriteGenerator::init();

  int i = 5;

  // cout << "create root renderTest" << endl;
  Element* proot;
  // cout << endl;

  switch (i) {
    case 0:
      proot = buildRootTestRectangle();
      break;

    case 1:
      proot = buildRootWindow();
      break;

    case 2:
      proot = buildRootWebpageStyle();
      break;

    case 3:
      proot = buildRootSprite();
      break;

    case 4:
      proot = testCopy();
      break;

    case 5:
      proot = CharacterStats();
      break;

    case 6:
      proot = testIconGenerator();
      break;

    default:
      proot = new WindowManager();
      break;
  }

  cout << proot->getTreeView() << endl;
  SpriteGenerator::Icon::pdefault->getSpriteUnit(0);

  createWindowWith(proot);
  delete proot;
}

void createWindowWith(Element* proot) {
  auto size = proot->getSizeAbs();

  sf::RenderWindow window(
      sf::VideoMode({(unsigned int)size.x, (unsigned int)size.y}),
      "Render Test");

  // window loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      auto mousePosAbsTemp = sf::Mouse::getPosition(window);
      sf::Vector2f mousePosAbs{(float)mousePosAbsTemp.x,
                               (float)mousePosAbsTemp.y};
      proot->reactEvent(event, mousePosAbs);
    }

    window.clear();
    window.draw(*proot);
    window.display();
  }
}

Element* buildRootTestRectangle() {
  WindowManager root;
  root.setSizeRelative({700, 700});

  Rectangle *pr1, *pr2, *pr3;
  pr1 = new Rectangle;
  pr2 = new Rectangle;
  pr3 = new Rectangle;
  root.add(pr1);
  root.add(pr2);
  root.add(pr3);

  pr1->setPosRelative({10, 20});
  pr1->setSizeRelative({200, 100});

  pr2->setPosRelative({250, 15});
  pr2->setSizeRelative({400, 300});

  pr3->setPosRelative({10, "60%"});
  pr3->setSizeRelative({"70%", 40});

  Rectangle *pr2_1, *pr2_2;
  pr2_1 = new Rectangle;
  pr2_2 = new Rectangle;
  pr2->add(pr2_1);
  pr2->add(pr2_2);

  pr2_1->recshape.setFillColor(sf::Color::Red);
  pr2_2->recshape.setFillColor(sf::Color::Red);

  pr2_1->setPosRelative({"-20%", "m"});
  pr2_1->setSizeRelative({40, 20});

  pr2_2->setPosRelative({80, 30});
  pr2_2->setSizeRelative({200, 200});

  Rectangle* pr2_2_1;
  pr2_2_1 = new Rectangle;
  pr2_2->add(pr2_2_1);
  pr2_2_1->recshape.setFillColor(sf::Color::Green);
  pr2_2_1->setPosRelative({-55, -55});
  pr2_2_1->setSizeRelative({50, 50});
  return root.getCopy();
}

Element* buildRootWebpageStyle() {
  WindowManager root;
  root.setSizeRelative({700, 700});

  auto pheader = new Rectangle(), pfooter = new Rectangle(),
       prightContent = new Rectangle(), plateralMenuLeft = new Rectangle();

  root.add(pheader);
  root.add(pfooter);
  root.add(plateralMenuLeft);
  root.add(prightContent);

  pheader->setSizeRelative({"100%", 50});

  pfooter->setSizeRelative({"100%", 50});
  pfooter->setPosRelative({0, "b"});

  plateralMenuLeft->setSizeRelative({"30%", -100});
  plateralMenuLeft->setPosRelative({0, 50});

  prightContent->setSizeRelative({"80%", -100});
  prightContent->setPosRelative({"30%", 50});

  auto ptitle = new Text();
  pheader->add(ptitle);
  ptitle->setString("Le Titre");
  ptitle->setPosRelative({"m", "m"});
  return root.getCopy();
}

Element* buildRootWindow() {
  WindowManager root;
  sf::Vector2f windowSizef = {700, 700};
  root.setSizeRelative({windowSizef.x, windowSizef.y});

  auto pwin = new Window();
  root.add(pwin);
  pwin->setPosRelative({50, 50});

  Window* pwin2 = static_cast<Window*>(pwin->getCopy());
  root.add(pwin2);

  cout << pwin2->getTreeView() << endl;
  pwin2->ptitleBar->recshape.setFillColor(sf::Color::Cyan);
  cout << "title bar color ok" << endl;
  pwin2->ptitle->text.setString("Win 2");
  cout << "title text ok" << endl;
  pwin2->pcloseBtn->recshape.setFillColor(sf::Color::Cyan);
  cout << "close btn color ok" << endl;
  pwin2->setPosRelative({400, 50});

  return root.getCopy();
}

Sprite* getSprite(int id) {
  static sf::Texture TextureStone, TextureWater;
  static bool TextureStoneInit = false, TextureWaterInit = false;
  static int width = 34, heigth = 24, nbRepeat = 3;
  static sf::IntRect area(0, 0, width, heigth);

  Sprite* psprite = new Sprite();
  if (id == 55) {  // stone
    if (!TextureStoneInit) {
      auto filename =
          "res/tileset/isometric_pixel_flat_00" + to_string(id) + ".png";
      TextureStone.loadFromFile(filename, area);
      TextureStone.setRepeated(true);
      TextureStoneInit = true;
    }
    psprite->sprite.setTexture(TextureStone);
  }

  else if (id == 28) {
    if (!TextureWaterInit) {
      auto filename =
          "res/tileset/isometric_pixel_flat_00" + to_string(id) + ".png";
      TextureWater.loadFromFile(filename, area);
      TextureStone.setRepeated(true);
      TextureWaterInit = true;
    }
    psprite->sprite.setTexture(TextureWater);
  }

  sf::IntRect textRect(0, 0, width * nbRepeat, heigth * nbRepeat);
  psprite->sprite.setTextureRect(textRect);
  psprite->updateSizeFromTextureRect();
  return psprite;
}

Element* buildRootSprite() {
  WindowManager root;
  sf::Vector2f windowSizef = {700, 700};
  root.setSizeRelative({windowSizef.x, windowSizef.y});

  auto pwin = new Window();
  root.add(pwin);
  pwin->setPosRelative({50, 50});
  pwin->setSizeRelative({500, 300});
  pwin->pcontent->recshape.setFillColor(sf::Color(50, 50, 50));

  pwin->pcontent->add(new Window());
  pwin->pcontent->add(new Window());

  auto pspriteStone = getSprite(55);
  pwin->pcontent->add(pspriteStone);
  pspriteStone->setPosRelative({0, 0});

  auto pspriteWater = getSprite(28);
  if (pspriteWater) {
    pwin->pcontent->add(pspriteWater);
    pspriteWater->setPosRelative({200, 100});
  }

  Element copy = root;
  cout << copy.getTreeView();
  cout << endl;

  return root.getCopy();
}

Element* testCopy() {
  cout << "create root" << endl;
  Element root;  // 0
  cout << endl;

  root.setSizeRelative({700, 700});

  Rectangle *psquare1, *psquare2, *psquare3;

  cout << "create square1" << endl;
  psquare1 = new Rectangle();  // 1
  cout << endl;

  psquare1->setSizeRelative({50, 50});
  psquare1->recshape.setFillColor(sf::Color::Red);

  cout << "create square2 from square1" << endl;
  psquare2 = new Rectangle(*psquare1);  // id = 3
  cout << endl;

  psquare2->setPosRelative({100, 100});
  psquare2->recshape.setFillColor(sf::Color::Blue);

  cout << "create square3" << endl;
  psquare3 = new Rectangle();  // id = 4
  cout << endl;

  cout << "square3 = square1" << endl;
  *psquare3 = *psquare1;
  cout << endl;

  psquare3->setPosRelative({200, 100});

  root.add(psquare1);
  root.add(psquare2);
  root.add(psquare3);
  root.add(psquare1->getCopy());

  Element& square4 = *(psquare2->getCopy());
  square4.setPosRelative({300, 300});
  square4.setSizeRelative({42, 42});
  root.add(square4.getCopy());

  cout << root.getTreeView();

  cout << "copy = root" << endl;
  Element* copy = root.getCopy();
  cout << endl;

  cout << copy->getTreeView();
  cout << endl;

  Rectangle* r = static_cast<Rectangle*>(copy->getChildren()[0]);
  r->recshape.setFillColor(sf::Color::Green);

  return copy;
}

Element* CharacterSheet() {
  Window CharacterSheet;
  CharacterSheet.ptitle->setString("Fiche de personnage");

  Text CharacterName;
  Sprite CharacterSprite;
  Element* CharacterStats;

  return CharacterSheet.getCopy();
}

Element* testIconGenerator() {
  Element root;
  root.setSizeRelative({1000, 1000});

  SpriteGeneratorById* iconGenerator = SpriteGenerator::Icon::pdefault;

  // Create icons
  Sprite iconpv, iconpa, iconpm;
  iconpv.sprite = iconGenerator->getSpriteUnit(IconType::pv);
  iconpa.sprite = iconGenerator->getSpriteUnit(IconType::pa);
  iconpm.sprite = iconGenerator->getSpriteUnit(IconType::pm);

  volatile auto test1 = iconpv.sprite.getScale();
  volatile auto test2 = iconpv.sprite.getTextureRect();
  volatile auto test3 = iconpv.sprite.getGlobalBounds();

  iconpv.updateSizeFromTextureRect();
  iconpa.updateSizeFromTextureRect();
  iconpm.updateSizeFromTextureRect();

  // place icon and add to root
  auto children = {iconpv, iconpa, iconpm};
  int y = 0;
  for (auto child : children) {
    child.setPosRelative({0, y});
    root.add(child.getCopy());
    y += child.getSizeAbs().y;
  }

  return root.getCopy();
}

Element* CharacterStats() {
  Rectangle container;

  container.recshape.setFillColor(sf::Color::White);

  // get generator of icons
  SpriteGeneratorById* iconGenerator = SpriteGenerator::Icon::pdefault;

  // load icons with generator
  Sprite iconpv, iconpa, iconpm;
  iconpv.sprite = iconGenerator->getSprite(IconType::pv, {20, 20});
  iconpa.sprite = iconGenerator->getSprite(IconType::pa, {20, 20});
  iconpm.sprite = iconGenerator->getSprite(IconType::pm, {20, 20});

  // test
  cout << "texture: " << iconpv.sprite.getTexture() << endl;

  // Create lines
  Element *linepv, *linepa, *linepm;
  linepv = StatLine("pv", iconpv, 20, 30);
  linepa = StatLine("pa", iconpa, 3, 6);
  linepm = StatLine("pm", iconpm, 2, 3);

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
                  Sprite statIcon,
                  int statValueCurrent,
                  int statValueMax) {
  // Assume that icon size is 20*20

  Element line;
  Text statNameText, statValues;

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