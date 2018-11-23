#include "WindowManager.h"
#include <iostream>
#include "Window.h"
#include "customElements/CharacterSheet.h"

using namespace std;
using namespace render;

WindowManager::WindowManager() {
  recshape.setFillColor(sf::Color::Transparent);
}

WindowManager::WindowManager(state::State* state) : WindowManager() {
  this->state = state;
}

void WindowManager::processEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::I) {
      toggleInventory();
    } else if (event.key.code == sf::Keyboard::T) {
      cout << getTreeView();
    } else if (event.key.code == sf::Keyboard::P) {
      toggleMainCharacter();
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

void WindowManager::toggleMainCharacter() {
  state::Character* pmainChar = this->state->getMainCharacter();
  openChar(pmainChar);
}

void WindowManager::openChar(state::Character* pchar) {
  if (find(openedChars.begin(), openedChars.end(), pchar) ==
      openedChars.end()) {
    Element* pcharSheet = CharacterSheet(pchar);
    pcharSheet->setPosRelative({"m", "m"});
    add(pcharSheet);
  }
}