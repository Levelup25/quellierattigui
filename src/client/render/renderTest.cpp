#include "renderTest.h"

void testRender() {
  int i = 2;
  Element* proot;
  switch (i) {
    case 1:
      proot = buildRootTestRectangle();
      break;

    case 1 << 1:
      proot = buildRootWindow();
      break;

    case 1 << 2:
      proot = buildRootWebpageStyle();
      break;

    case 1 << 3:
      proot = buildRootSprite();
      break;

    default:
      proot = new WindowManager();
      break;
  }

  createWindowWith(proot);
}

void createWindowWith(Element* root) {
  auto size = root->getSizeAbs();
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
      root->reactEvent(event, mousePosAbs);
    }

    window.clear();
    window.draw(*root);
    window.display();
  }
  delete root;
}

Element* buildRootTestRectangle() {
  auto root = new WindowManager();
  root->setSizeRelative({700, 700});
  root->recshape.setFillColor(sf::Color::Black);

  Rectangle *pr1, *pr2, *pr3;
  pr1 = new Rectangle;
  pr2 = new Rectangle;
  pr3 = new Rectangle;
  root->add(pr1);
  root->add(pr2);
  root->add(pr3);

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

Element* buildRootWebpageStyle() {
  auto root = new WindowManager();
  root->setSizeRelative({700, 700});
  root->recshape.setFillColor(sf::Color::Black);

  auto pheader = new Rectangle(), pfooter = new Rectangle(),
       prightContent = new Rectangle(), plateralMenuLeft = new Rectangle();

  root->add(pheader);
  root->add(pfooter);
  root->add(plateralMenuLeft);
  root->add(prightContent);

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

Element* buildRootWindow() {
  auto root = new WindowManager();
  sf::Vector2f windowSizef = {700, 700};
  root->setSizeRelative({windowSizef.x, windowSizef.y});
  root->recshape.setFillColor(sf::Color::Black);

  auto pwin = new Window();
  root->add(pwin);
  pwin->setPosRelative({50, 50});
  return root;
}

Element* buildRootSprite() {
  auto root = new WindowManager();
  sf::Vector2f windowSizef = {700, 700};
  root->setSizeRelative({windowSizef.x, windowSizef.y});
  root->recshape.setFillColor(sf::Color::Black);
  return root;
}