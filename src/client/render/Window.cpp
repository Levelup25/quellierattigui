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

void Window::reactEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  // todo move to init witdh function pointer
  auto pclose = ptitleBar->pcloseBtn;
  auto mouseOverClose = pclose->isInside(mousePosAbs);
  if (mouseOverClose) {
    pclose->recshape.setFillColor(sf::Color::Red);
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      setWaitingDestruction(true);
    }
  } else {
    pclose->recshape.setFillColor(sf::Color::White);
  }

  static sf::Vector2f mouseOffset = {0, 0};
  static bool isDraging = false;
  if (isDraging) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      auto ppos = getParent()->getPosAbs();
      sf::Vector2f posRelativNew = (mousePosAbs - mouseOffset) - ppos;
      posRelativNew.x = posRelativNew.x < 0 ? 0 : posRelativNew.x;
      posRelativNew.y = posRelativNew.y < 0 ? 0 : posRelativNew.y;
      setPosRelative(posRelativNew);
    } else {
      isDraging = false;
    }
  } else if (event.type == sf::Event::MouseButtonPressed &&
             event.mouseButton.button == sf::Mouse::Left && !isDraging &&
             !mouseOverClose) {
    isDraging = true;
    mouseOffset = mousePosAbs - getPosAbs();
  }
}
