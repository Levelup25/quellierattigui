#include "renderTest.h"
#include <iostream>
#include "Rectangle.h"
#include "Window.h"
#include "WindowManager.h"

using namespace render;
using namespace std;

void testRender() {
  sf::RenderWindow window(sf::VideoMode(700, 700), "Render Test");
  auto wm = WindowManager();
  auto w1 = Window();
  wm.add(&w1);
  w1.setPos({50, 50});

  cout << "wm : " << wm.posToStr(wm.getAbsPos()) << endl;
  cout << "w1 : " << wm.posToStr(w1.getAbsPos()) << endl;

  auto w1border = *w1.pborder;
  auto w1bar = *w1.ptitleBar;
  auto w1barBorder = *w1bar.pborder;
  cout << "w1 border : " << wm.posToStr(w1border.getAbsPos()) << endl;
  cout << "w1 bar : " << wm.posToStr(w1bar.getAbsPos()) << endl;
  cout << "w1 bar border : " << wm.posToStr(w1barBorder.getAbsPos()) << endl;

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
