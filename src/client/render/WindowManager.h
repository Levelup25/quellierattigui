// Generated by dia2code
#ifndef RENDER__WINDOWMANAGER__H
#define RENDER__WINDOWMANAGER__H

#include <SFML/Graphics.hpp>

namespace render {
  class Inventory;
  class Rectangle;
}

#include "Rectangle.h"

namespace render {

  /// class WindowManager - 
  class WindowManager : public render::Rectangle {
    // Attributes
  protected:
    Inventory* inventory;
    // Operations
  public:
    WindowManager ();
    void reactEvent (sf::Event event, sf::Vector2f posMouse);
  private:
    void toggleInventory ();
    // Setters and Getters
    const Inventory*& getInventory() const;
    void setInventory(const Inventory*& inventory);
  };

};

#endif