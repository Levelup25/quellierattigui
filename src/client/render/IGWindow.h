#ifndef IGWINDOW_H
#define IGWINDOW_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "colors.h"

// In Game Windows
class IGWindow : public sf::Drawable {
 protected:
  // Properties
  sf::Vector2f pos, size;
  sf::String title = "";

  // Const display
  const float headerHeight = 30;
  const float barBtnSize = 20;
  const float barPad = 10;
  sf::Font roboto;

  // Event
  sf::Vector2f posLastPressed;
  sf::Vector2f posLastRealeased;
  sf::Vector2f offsetMouse;
  void startClosing();

  // Utility methods
  bool posInRec(sf::Vector2f pos, sf::RectangleShape rec);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  // sfml components
  sf::RectangleShape sfHeader;
  sf::RectangleShape sfCloseBtn;
  sf::RectangleShape sfContent;
  sf::Text sfTitle;

  // Event tag (read by parent/window manager for closing or put first, etc..)
  bool isDragging = false;
  bool displayFirst = false;
  bool isClosing = false;

  // Constructor, destructor
  IGWindow();
  ~IGWindow();

  // Receive and process event
  void receiveEvent(sf::Event, sf::Vector2f posMouse);

  // getters & setters
  virtual void setPosition(sf::Vector2f newPos);
  void setSize(sf::Vector2f newSize);
  void setSizeContent(sf::Vector2f newSize);
  void setTitle(std::string newTitle);
  sf::Vector2f getPosition();
  sf::Vector2f getSize();
  sf::Vector2f getSizeContent();
  sf::String getTitle();
};

#endif  // IGWINDOW_H