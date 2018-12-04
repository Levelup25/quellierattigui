#include "Window.h"
#include <iostream>
#include <sstream>
#include "Rectangle.h"

using namespace render;
using namespace std;

Window::Window() {
  setSizeRelative({200, 100});
  ptitleBar = new Rectangle();
  ptitle = new Text();
  pcloseBtn = new Rectangle();
  pcontent = new Rectangle();

  add(ptitleBar);
  add(pcontent);
  ptitleBar->add(ptitle);
  ptitleBar->add(pcloseBtn);

  float titleBarHeight = 30;
  pcontent->setPosRelative({0, titleBarHeight});
  pcontent->setSizeRelative({"100%", -titleBarHeight});

  ptitleBar->recshape.setFillColor(sf::Color::Blue);
  ptitleBar->setSizeRelative({"100%", titleBarHeight});

  ptitle->text.setString("Window " + to_string(getId()));
  ptitle->setPosRelative({5, 5});
  ptitle->text.setColor(sf::Color::White);
  ptitle->text.setCharacterSize(16);

  pcloseBtn->setPosRelative({-5, -5});
  pcloseBtn->setSizeRelative({20, 20});
}

Window::Window(const Window& win) : Rectangle(win) {
  *this = win;
}

Window& Window::operator=(const Window& win) {
  // WARNING: What happens if this elements switch pos?
  ptitleBar = static_cast<Rectangle*>(getChildren()[0]);
  pcontent = static_cast<Rectangle*>(getChildren()[1]);
  ptitle = static_cast<Text*>(ptitleBar->getChildren()[0]);
  pcloseBtn = static_cast<Rectangle*>(ptitleBar->getChildren()[1]);
  return *this;
}

Element* Window::getCopy() const {
  return new Window(*this);
}

void Window::processEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  for (auto pchild : pcontent->getChildren()) {
    if (pchild->getLink() != nullptr && pchild->getMouseOver()) {
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        Element* root;
        do {
          root = this->getParent();
        } while (root->getParent() != nullptr);
        if (!root->getChild(pchild->getLink()))
          root->add(pchild->getLink());
      }
    }
  }

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
  }

  // Close button (hover and close)
  if (pcloseBtn->getMouseOver() && !isDraging) {
    pcloseBtn->recshape.setFillColor(sf::Color::Red);
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      setWaitingDestruction(true);
    }
  } else {
    pcloseBtn->recshape.setFillColor(sf::Color::White);
  }

  // if over windows or child
  if (getMouseOver() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
    getParent()->moveAtTop(this);
  }

  // start Dragging
  if (ptitleBar->getMouseOver() && !pcloseBtn->getMouseOver() &&
      event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left && !isDraging) {
    isDraging = true;
    mouseOffset = mousePosAbs - getPosAbs();
  }
}
