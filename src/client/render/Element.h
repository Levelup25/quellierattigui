// Generated by dia2code
#ifndef RENDER__ELEMENT__H
#define RENDER__ELEMENT__H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace render {
  class Element;
};
namespace sf {
  class Drawable;
}


namespace render {

  /// class Element - 
  class Element : public sf::Drawable {
    // Attributes
  private:
    Element* pparent     = nullptr;
    std::vector<Element*> pchildren;
    unsigned int depth     = 0;
    sf::Vector2f posRelative;
    sf::Vector2f size;
    // Operations
  public:
    Element ();
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void setPosRelative (const sf::Vector2f pos);
    const sf::Vector2f getPosRelative () const;
    const sf::Vector2f getAbsPos () const;
    std::string posToStr (sf::Vector2f pos) const;
    virtual void setSize (const sf::Vector2f size);
    const sf::Vector2f getSize () const;
    void notifyPos ();
    void notifySize ();
    void notifyEvent (sf::Event event, sf::Vector2f posMouse);
    virtual void updatePosParent ();
    virtual void updateSizeParent ();
    virtual void updateEvent (sf::Event event, sf::Vector2f posMouse);
    void add (Element* pchild);
    void remove (Element* pchild);
    void setParent (Element* pparent);
    const Element* getParent () const;
    unsigned int getDepth () const;
    void printTreeView () const;
  private:
    void updateDepth ();
    // Setters and Getters
  };

};

#endif
