#ifndef IGWINDOW_H
#define IGWINDOW_H

#include <SFML/Graphics.hpp>
#include <iostream>

// In Game Windows
class IGWindow : public sf::Drawable {
 protected:
  // Properties
  sf::Vector2f pos, size;
  sf::String title = "";
  const float headerHeight = 30;
  const float barBtnSize = 20;
  const float barPad = 10;
  sf::Font roboto;

  // Event
  sf::Vector2f posLastPressed;
  sf::Vector2f posLastRealeased;
  sf::Vector2f offsetMouse;
  bool isDragging = false;
  void startClosing();
  bool isClosing = false;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  // sfml component
  sf::RectangleShape sfHeader;
  sf::RectangleShape sfCloseBtn;
  sf::RectangleShape sfContent;
  sf::Text sfTitle;

  IGWindow();
  ~IGWindow();

  bool posInRec(sf::Vector2f pos, sf::RectangleShape rec);
  bool isMouseOver();
  void receiveEvent(sf::Event, sf::Vector2f posMouse);

  // getter setter
  void setPosition(sf::Vector2f newPos);
  sf::Vector2f getPosition();
  void setSize(sf::Vector2f newSize);
  sf::Vector2f getSize();
  void setTitle(std::string newTitle);
  sf::String getTitle();
  bool getIsClosing();
};

#endif  // IGWINDOW_H