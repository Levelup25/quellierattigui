#include "Cell.h"

using namespace std;
using namespace state;

Cell::Cell(ElementType element, ContentType content) {
  this->tileElement = element;
  this->tileContent = content;
}

void Cell::setElement(ElementType element) {
  tileElement = element;
}

ElementType Cell::getElement() {
  return tileElement;
}

void Cell::setContent(ContentType content) {
  tileContent = content;
}

ContentType Cell::getContent() {
  return tileContent;
}
