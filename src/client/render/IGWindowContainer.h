#ifndef IGWINDOW_CONTAINER_H
#define IGWINDOW_CONTAINER_H

#include <SFML/Graphics.hpp>
#include "IGWindow.h"
#include "colors.h"

class IGWindowContainer : public sf::Drawable {
 protected:
  std::vector<IGWindow*> winStack;
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  void add(IGWindow* pwindow);
  void remove(IGWindow* pwindow);
  void putAtEnd(IGWindow* pwindow);
  void transmit(sf::Event event, sf::Vector2f mousePos);
  std::vector<IGWindow*> getWinStack();

  void processChildrenTags();
};

#endif