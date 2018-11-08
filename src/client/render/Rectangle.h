// Generated by dia2code
#ifndef RENDER__RECTANGLE__H
#define RENDER__RECTANGLE__H

#include <SFML/Graphics.hpp>

namespace render {
  class Element;
}

#include "Element.h"

namespace render {

  /// class Rectangle - 
  class Rectangle : public render::Element {
    // Attributes
  public:
    sf::RectangleShape recshape;
    // Operations
  public:
    Rectangle ();
    void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    void setPos (const sf::Vector2f pos);
    void setSize (const sf::Vector2f size);
    // Setters and Getters
  };

};

#endif
