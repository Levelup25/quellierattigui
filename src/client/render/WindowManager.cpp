#include "WindowManager.h"
#include <iostream>
#include "Window.h"

using namespace std;
using namespace render;

WindowManager::WindowManager() {
  recshape.setFillColor(sf::Color::Transparent);
}

void WindowManager::processEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::I) {
      toggleInventory();
    } else if (event.key.code == sf::Keyboard::T) {
      cout << getTreeView();
    } else if (event.key.code == sf::Keyboard::N) {
      auto pw = new Window();
      add(pw);
      pw->setPosRelative({10, 10});
    }
  }
}

void WindowManager::toggleInventory() {
  cout << "Inventory open" << endl;
  // TODO
}
