#include "renderTest.h"
#include <iostream>
#include "Rectangle.h"
#include "Window.h"
#include "WindowManager.h"

using namespace render;
using namespace std;

void testRender() {
  int i = 1;
  switch (i) {
    case 0:
      testPosition();
      break;

    case 1:
      testDisplayWindow();
      break;

    default:
      break;
  }
}

void testPosition() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Render Test");

  Rectangle root;
  sf::Vector2f windowSizef = {(float)window.getSize().x,
                              (float)window.getSize().y};
  root.setSizeRelative({windowSizef.x, windowSizef.y});
  root.recshape.setFillColor(sf::Color::Black);

  Rectangle *pr1, *pr2, *pr3;
  root.add(pr1);
  root.add(pr2);
  root.add(pr3);
  pr1->setPosRelative({10, 20});
  pr1->setSizeRelative({200, 100});
  pr2->setPosRelative({250, 15});
  pr2->setSizeRelative({400, 300});
  pr1->setSizeRelative({200, 100});
  pr3->setPosRelative({10, "60%"});
  pr3->setSizeRelative({"70%", 40});

  Rectangle *pr2_1, *pr2_2;
  pr2->add(pr2_1);
  pr2->add(pr2_2);
  pr1->recshape.setFillColor(sf::Color::Red);
  pr2->recshape.setFillColor(sf::Color::Red);
  pr1->setPosRelative({"-20%", "m"});
  pr1->setSizeRelative({40, 20});
  pr2->setPosRelative({80, 30});
  pr2->setSizeRelative({200, 200});

  Rectangle* pr2_2_1;
  pr2->add(pr2_2_1);
  pr1->recshape.setFillColor(sf::Color::Green);
  pr1->setPosRelative({-55, -55});
  pr1->setSizeRelative({50, 50});

  cout << root.getTreeView();

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

void testDisplayWindow() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Render Test");

  Rectangle root;
  sf::Vector2f windowSizef = {(float)window.getSize().x,
                              (float)window.getSize().y};
  root.setSizeRelative({windowSizef.x, windowSizef.y});
  root.recshape.setFillColor(sf::Color::Black);

  auto pwin = new Window();
  root.add(pwin);
  pwin->setPosRelative({50, 50});

  cout << root.getTreeView();

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