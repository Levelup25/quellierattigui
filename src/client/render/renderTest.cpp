#include "renderTest.h"

void testRender() {
  int i = 3;

  // cout << "create root renderTest" << endl;
  Element root;
  // cout << endl;

  switch (i) {
    case 0:
      root = buildRootTestRectangle();
      break;

    case 1:
      root = buildRootWindow();
      break;

    case 2:
      root = buildRootWebpageStyle();
      break;

    case 3:
      root = buildRootSprite();
      break;

    case 4:
      root = testCopy();
      break;

    default:
      root = WindowManager();
      break;
  }
  createWindowWith(root);
}

void createWindowWith(Element root) {
  auto size = root.getSizeAbs();
  cout << root.getTreeView() << endl;

  // auto pchildren = root.getChildren();
  // Rectangle* prec = dynamic_cast<Rectangle*>(pchildren[0]);
  // auto test = prec->recshape.getSize();
  // cout << test.x << " " << test.y << endl;

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
      root.reactEvent(event, mousePosAbs);
    }

    window.clear();
    window.draw(root);
    window.display();
  }
}

Element buildRootTestRectangle() {
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
  return root;
}

Element buildRootWebpageStyle() {
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
  return root;
}

Element buildRootWindow() {
  WindowManager root;
  sf::Vector2f windowSizef = {700, 700};
  root.setSizeRelative({windowSizef.x, windowSizef.y});

  auto pwin = new Window();
  root.add(pwin);
  pwin->setPosRelative({50, 50});
  return root;
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

Element buildRootSprite() {
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

  return root;
}

Element testCopy() {
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

  Element t;
  t.setSizeRelative({42, 42});
  root.add(t.getCopy());

  cout << root.getTreeView();

  cout << "copy = root" << endl;
  Element copy = root;
  cout << endl;

  cout << copy.getTreeView();
  cout << endl;

  return copy;
}