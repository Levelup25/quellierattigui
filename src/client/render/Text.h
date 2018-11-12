// Generated by dia2code
#ifndef RENDER__TEXT__H
#define RENDER__TEXT__H

#include <SFML/Graphics.hpp>
#include <string>

namespace render {
  class Element;
}

#include "Element.h"

namespace render {

  /// class Text - 
  class Text : public render::Element {
    // Attributes
  public:
    sf::Text text;
    static sf::Font roboto;
  private:
    static bool robotoLoaded;
    // Operations
  public:
    Text ();
    ~Text ();
    void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    void updatePosAbs ();
    void updateSizeAbs ();
    void setString (std::string str);
    // Setters and Getters
  };

};

#endif
