#include "renderTest.h"
//#include "IGWindow.h"
//#include "IGWindowContainer.h"
//#include "UIInventory.h"

void testRender() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Render Test");

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
    window.display();
  }
}
