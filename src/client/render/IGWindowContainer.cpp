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

void IGWindowContainer::putAtEnd(IGWindow* pwindow) {
  for (auto it = winStack.begin(); it != winStack.end(); ++it) {
    auto pwk = *it;
    if (pwk == pwindow) {
      std::rotate(it, it + 1, winStack.end());
      return;
    }
  }
}

void IGWindowContainer::transmit(sf::Event event, sf::Vector2f posMouse) {
  for (auto pw : winStack) {
    pw->receiveEvent(event, posMouse);
  }
  processChildrenTags();
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

void IGWindowContainer::processChildrenTags() {
  auto it = winStack.begin();
  while (it != winStack.end()) {
    auto pw = *it;
    if (pw->isClosing)
      remove(pw);

    else if (pw->displayFirst) {
      putAtEnd(pw);
      pw->displayFirst = false;
    }

    it++;
  }
}