// Generated by dia2code
#ifndef STATE__CELL__H
#define STATE__CELL__H


#include "ContentType.h"
#include "ElementType.h"

namespace state {

  /// class Cell - 
  class Cell {
    // Associations
    // Attributes
  private:
    ContentType tileContent;
    ElementType tileElement;
    // Operations
  public:
    Cell (ElementType element = neutral, ContentType content = nothing);
    void setElement (ElementType element);
    ElementType getElement ();
    void setContent (ContentType content);
    ContentType getContent ();
    // Setters and Getters
  };

};

#endif
