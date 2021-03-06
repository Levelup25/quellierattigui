// Generated by dia2code
#ifndef RENDER__TEXT__H
#define RENDER__TEXT__H

#include <SFML/Graphics.hpp>
#include <string>

namespace render {
  class Text;
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
    Text (const Text& obj);
    Text& operator= (const Text& obj);
    void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    void updatePosAbs ();
    void setString (std::string str);
    virtual Element* getCopy () const;
    // Setters and Getters
  };

};

#endif
