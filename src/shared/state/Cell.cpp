#include "Cell.h"

namespace state {

    Cell::Cell(ElementType element, ContentType content) {
      this->tileContent = nothing;
      this->tileElement = neutral;
    }
    
    void Cell::setElement (ElementType element) {
        tileElement=element;
    }
    
    ElementType Cell::getElement () {
        return tileElement;
    }
    
    
    void Cell::setContent (ContentType content) {
        tileContent=content;
    }
    
    ContentType Cell::getContent () {
        return tileContent;
    }
}  // namespace state
