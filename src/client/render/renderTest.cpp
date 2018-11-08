#include "renderTest.h"
#include <iostream>
#include "Rectangle.h"
#include "Window.h"
#include "WindowManager.h"

using namespace render;
using namespace std;

void testRender() {
  testRenderPosition();
}

void testRenderPosition() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Render Test");

  Rectangle root;
  sf::Vector2f windowSizef = {(float)window.getSize().x,
                              (float)window.getSize().y};
  root.setSize(windowSizef);
  root.recshape.setFillColor(sf::Color::Black);

  Rectangle r1, r2, r3;
  root.add(&r1);
  root.add(&r2);
  root.add(&r3);
  r1.setPos({10, 20});
  r1.setSize({200, 100});
  r2.setPos({250, 15});
  r2.setSize({400, 300});
  r1.setSize({200, 100});
  r3.setPos({10, -50});
  r3.setSize({400, 40});

  Rectangle r2_1, r2_2;
  r2.add(&r2_1);
  r2.add(&r2_2);
  r2_1.recshape.setFillColor(sf::Color::Red);
  r2_2.recshape.setFillColor(sf::Color::Red);
  r2_1.setPos({10, 20});
  r2_1.setSize({40, 20});
  r2_2.setPos({80, 30});
  r2_2.setSize({200, 200});

  Rectangle r2_2_1;
  r2_2.add(&r2_2_1);
  r2_2_1.recshape.setFillColor(sf::Color::Green);
  r2_2_1.setPos({-55, -55});
  r2_2_1.setSize({50, 50});

  root.printTreeView();

  // window loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      auto posMouseBuff = sf::Mouse::getPosition(window);
      sf::Vector2f posMouse{(float)posMouseBuff.x, (float)posMouseBuff.y};
    }

    window.clear();
    window.draw(root);
    window.display();
  }
}
