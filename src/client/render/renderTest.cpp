#include "renderTest.h"
#include "IGWindow.h"
#include "IGWindowContainer.h"
#include "UIInventory.h"

void testRender2() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Test rendu");
  IGWindowContainer wcontainer;

  float space = 10, width = 150;
  size_t row = 3, col = 4;

  // create and pos subWindows
  for (size_t i = 0; i < row; i++) {
    for (size_t j = 0; j < col; j++) {
      IGWindow* pw = new IGWindow();
      sf::Vector2f pos = sf::Vector2f((j + 1) * space + j * width,
                                      (i + 1) * space + i * width);
      pw->setPosition(pos);
      pw->setSize(sf::Vector2f(width, width));
      std::string title = "Test" + std::to_string(i) + std::to_string(j);
      pw->setTitle(title);

      wcontainer.add(pw);
    }
  }

  auto pw2 = wcontainer.getWinStack()[2];
  wcontainer.putAtEnd(pw2);

  // inventory
  UIInventory inv;
  inv.setPosition({30, 30});
  wcontainer.add(&inv);

  // window loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      auto posMouseBuff = sf::Mouse::getPosition(window);
      sf::Vector2f posMouse{(float)posMouseBuff.x, (float)posMouseBuff.y};
      wcontainer.transmit(event, posMouse);
    }

    window.clear();
    window.draw(wcontainer);
    window.display();
  }
}
