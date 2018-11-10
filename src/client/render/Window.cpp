#include "Window.h"
#include <iostream>
#include "Rectangle.h"

using namespace render;
using namespace std;

Window::Window() {
  setSizeRelative({100, 100});
  ptitleBar = new TitleBar();
  add(ptitleBar);

  auto prec = new Rectangle();
  prec->recshape.setFillColor(sf::Color(5));
  pcontent = prec;

  auto TitleBarSize = ptitleBar->getSizeAbs();
  pcontent->setPosRelative({0, TitleBarSize.y});
  pcontent->setSizeRelative({"100%", -TitleBarSize.y});
  add(pcontent);
}

void Window::reactEvent(sf::Event event, sf::Vector2f posMouse) {
  auto pclose = ptitleBar->pcloseBtn;
  if (pclose->isInside(posMouse)) {
    pclose->recshape.setFillColor(sf::Color::Red);
  } else {
    pclose->recshape.setFillColor(sf::Color::White);
  }
}
