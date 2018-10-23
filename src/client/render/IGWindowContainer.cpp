#include "IGWindowContainer.h"

void IGWindowContainer::add(IGWindow* pwindow) {
  winStack.push_back(pwindow);
}

void IGWindowContainer::remove(IGWindow* pwindow) {
  for (auto it = winStack.begin(); it != winStack.end(); ++it) {
    auto pwk = *it;
    if (pwk == pwindow) {
      winStack.erase(it);
      pwk->~IGWindow();
      return;
    }
  }
}

void IGWindowContainer::transmit(sf::Event event) {
  for (auto pw : winStack) {
    pw->receiveEvent(event);
  }
  autoclose();
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

void IGWindowContainer::autoclose() {
  for (auto pw : winStack) {
    if (pw->getIsClosing()) {
      remove(pw);
      autoclose();
      return;
    }
  }
}