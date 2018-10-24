#ifndef UIINVENTORY_H
#define UIINVENTORY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "IGWindow.h"
#include "colors.h"

class UIInventory : public IGWindow {
 private:
  // Properties
  // inventaire state

  // Const display
  const size_t nbRow = 10, nbCol = 30;
  const float footerHeight = 20;
  const float bodyPadding = 10;
  const sf::Vector2f caseSize = {16, 16};
  const float contentPadding[4] = {10, 10, 10, 10};  // top, bottom, left, right
  const float vspace = 10;  // space between vertical element
  const unsigned int footerTextHight = 15;

  // calulated properties
  size_t getNbItems() const;  // = nbCol * nbRow;

  // sfml component
  std::vector<sf::RectangleShape> sfcases;
  sf::Text sffooterText;

  // absolute pos
  sf::Vector2f getPosGrid() const;        // calculated
  sf::Vector2f getPosFooterText() const;  // calculated

  // relative pos
  const sf::Vector2f posGrid2Content;
  const sf::Vector2f posFooterText2Content;  // calculated: posGrid2Content +
                                             // heiht grid + vspace

  // drow
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 public:
  UIInventory();

  void setPosition(sf::Vector2f newPos);
};

#endif  // UIINVENTORY_H