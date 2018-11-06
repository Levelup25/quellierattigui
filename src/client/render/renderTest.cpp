#include "renderTest.h"
#include "WindowManager.h"

using namespace render;

void testRender() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Render Test");
  auto wm = WindowManager();

  // window loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      auto posMouseBuff = sf::Mouse::getPosition(window);
      sf::Vector2f posMouse{(float)posMouseBuff.x, (float)posMouseBuff.y};

      wm.receiveEvent(event, posMouse);
    }

    window.clear();
    window.draw(wm);
    window.display();
  }
}
