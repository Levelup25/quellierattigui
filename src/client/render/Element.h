// Generated by dia2code
#ifndef RENDER__ELEMENT__H
#define RENDER__ELEMENT__H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <ostream>

namespace render {
  class Element;
  class Relatif2;
  class Relatif;
};
namespace sf {
  class Drawable;
}

#include "Relatif2.h"
#include "Relatif.h"

namespace render {

  /// class Element - 
  class Element : public sf::Drawable {
    // Associations
    // Attributes
  private:
    Element* pparent     = nullptr;
    std::vector<Element*> pchildren;
    unsigned int depthCache     = 0;
    Relatif2 posRelative;
    sf::Vector2f posAbsCache;
    Relatif2 sizeRelative;
    sf::Vector2f sizeAbsCache;
    // Operations
  public:
    Element ();
    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const;
    const Relatif2 getPosRelative () const;
    virtual void setPosRelative (Relatif2 posRelatif);
    virtual void updatePosAbs ();
    const sf::Vector2f getPosAbs () const;
    virtual void reactEditPosAbsParent ();
    void notifyEditPosAbs ();
    const Relatif2 getSizeRelative () const;
    void setSizeRelative (Relatif2 sizeRelatif);
    virtual void updateSizeAbs ();
    const sf::Vector2f getSizeAbs () const;
    void notifyEditSizeAbs ();
    virtual void reactEditSizeAbsParent ();
    void notifyEvent (sf::Event event, sf::Vector2f posMouse);
    virtual void reactEvent (sf::Event event, sf::Vector2f posMouse);
    const Element* getParent () const;
    void setParent (Element* pparent);
    void add (Element* pchild);
    void remove (Element* pchild);
    unsigned int getDepth () const;
    std::string posToStr (sf::Vector2f pos) const;
    std::string getTreeView () const;
    friend std::ostream& operator<< (std::ostream& os, const Element& el);
    friend std::ostream& operator<< (std::ostream& os, const sf::Vector2f& vec);
  private:
    void updateDepth ();
    float computeCoord (Relatif rel, float parentCoordAbs, float parentLengthAbs, float lengthAbs);
    float computeLength (Relatif rel, float parentLengthAbs);
    // Setters and Getters
  };

};

#endif
