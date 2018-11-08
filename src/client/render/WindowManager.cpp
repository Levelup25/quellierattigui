#include "WindowManager.h"
#include <iostream>

using namespace std;
using namespace render;

WindowManager::WindowManager() {}

void WindowManager::updateEvent(sf::Event event, sf::Vector2f posMouse) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::I) {
      toggleInventory();
    } else if (event.key.code == sf::Keyboard::T) {
      cout << "the T key was pressed" << endl;
    }
  }
}

void WindowManager::toggleInventory() {
  cout << "Inventory open" << endl;
  // TODO
}
