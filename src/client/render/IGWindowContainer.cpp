#include "IGWindowContainer.h"

void IGWindowContainer::add(IGWindow* pwindow) {
  winStack.push_back(pwindow);
}

void IGWindowContainer::remove(IGWindow* pwindow) {
  for (auto it = winStack.begin(); it != winStack.end(); ++it) {
    if (*it == pwindow) {
      winStack.erase(it);
      return;
    }
  }
}

void IGWindowContainer::transmit(sf::Event event) {
  for (auto pw : winStack) {
    pw->receiveEvent(event);
  }
}

void IGWindowContainer::draw(sf::RenderTarget& target,
                             sf::RenderStates states) const {
  for (auto pw : winStack) {
    target.draw(*pw);
  }
}

std::vector<IGWindow*> IGWindowContainer::getWinStack() {
  return winStack;
}