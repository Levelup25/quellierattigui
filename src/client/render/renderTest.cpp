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

  Rectangle r1, r2, r3;
  root.add(&r1);
  root.add(&r2);
  root.add(&r3);
  r1.setPosRelative({10, 20});
  r1.setSizeRelative({200, 100});
  r2.setPosRelative({250, 15});
  r2.setSizeRelative({400, 300});
  r1.setSizeRelative({200, 100});
  r3.setPosRelative({10, "60%"});
  r3.setSizeRelative({"70%", 40});

  Rectangle r2_1, r2_2;
  r2.add(&r2_1);
  r2.add(&r2_2);
  r2_1.recshape.setFillColor(sf::Color::Red);
  r2_2.recshape.setFillColor(sf::Color::Red);
  r2_1.setPosRelative({"-20%", "m"});
  r2_1.setSizeRelative({40, 20});
  r2_2.setPosRelative({80, 30});
  r2_2.setSizeRelative({200, 200});

  Rectangle r2_2_1;
  r2_2.add(&r2_2_1);
  r2_2_1.recshape.setFillColor(sf::Color::Green);
  r2_2_1.setPosRelative({-55, -55});
  r2_2_1.setSizeRelative({50, 50});

  cout << root.getTreeView();

  // window loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      auto posMouseTemp = sf::Mouse::getPosition(window);
      sf::Vector2f posMouse{(float)posMouseTemp.x, (float)posMouseTemp.y};
      root.reactEvent(event, posMouse);
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

  Window win;
  root.add(&win);
  win.setPosRelative({50, 50});

  cout << root.getTreeView();

  // window loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      auto posMouseTemp = sf::Mouse::getPosition(window);
      sf::Vector2f posMouse{(float)posMouseTemp.x, (float)posMouseTemp.y};
      root.reactEvent(event, posMouse);
    }

    window.clear();
    window.draw(root);
    window.display();
  }
}