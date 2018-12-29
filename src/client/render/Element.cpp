#include "Element.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <typeinfo>

#define DEBUGING 0
#define IDDEBUG -1

using namespace render;
using namespace std;

int Element::created = 0;

Element::Element() {
  id = Element::created++;

  posRelative = {0, 0};
  sizeRelative = {10, 10};

  if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
    cout << ">>>create: " << id << endl;
  }
};

Element::Element(const Element& obj) : Element() {
  *this = obj;
}

Element& Element::operator=(const Element& obj) {
  // WARNING: don't copy parent, its a normal behaviour, but copy children

  posRelative = obj.getPosRelative();
  sizeRelative = obj.getSizeRelative();
  posAbsCache = obj.getPosAbs();
  sizeAbsCache = obj.getSizeAbs();
  mouseOverCache = obj.getMouseOver();
  link = obj.link;

  pchildren = {};
  for (auto pchildObj : obj.getChildren()) {
    Element* pchild = pchildObj->getCopy();
    pchild->setParent(this);
    pchildren.push_back(pchild);
  }

  return *this;
}

Element* Element::getCopy() const {
  return new Element(*this);
}

Element::~Element() {
  for (auto pchild : pchildren) {
    pchild->~Element();
  }
}

void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (int i = pchildren.size(); i-- > 0;) {
    auto pchild = pchildren[i];
    target.draw(*pchild);
  }
};

void Element::reactEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  if (!pparent) {
    mouseOverCache = isInside(mousePosAbs);
  }
  if (mouseOverCache && event.type == sf::Event::KeyPressed &&
      event.key.code == sf::Keyboard::D) {
    cout << ">>>Debug " << this << " " << link << endl;
    cout << *this << endl;
  }
  processEvent(event, mousePosAbs);
  closeChildrenWaiting();
  updateChildrenMouseOver(mousePosAbs);
  notifyEvent(event, mousePosAbs);
};

void Element::notifyEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  for (auto pchild : pchildren) {
    pchild->reactEvent(event, mousePosAbs);
  }
}

void Element::closeChildrenWaiting() {
  auto it = pchildren.begin();
  while (it != pchildren.end()) {
    auto pchild = *it;
    if (pchild->waitingDestruction) {
      it = pchildren.erase(it);
      pchild->setParent(nullptr);
      pchild->~Element();
    } else {
      pchild->closeChildrenWaiting();
      it++;
    }
  }
}

void Element::updateChildrenMouseOver(sf::Vector2f mousePosAbs) {
  volatile size_t l = pchildren.size();
  for (size_t i = 0; i < pchildren.size(); i++) {
    auto pchild = pchildren[i];
    if (!getMouseOver())
      pchild->setMouseOver(false, mousePosAbs);

    else {
      if (i < l && pchild->isInside(mousePosAbs)) {
        pchild->setMouseOver(true, mousePosAbs);
        l = i;
      } else {
        pchild->setMouseOver(false, mousePosAbs);
      }
    }
  }
}

const Relatif2 Element::getPosRelative() const {
  return posRelative;
}

void Element::setPosRelative(Relatif2 pos) {
  if (pparent && (pos.x.getComputeMethod() != ComputeMethodType::pixel ||
                  pos.y.getComputeMethod() != ComputeMethodType::pixel)) {
    sf::Vector2f parentSizeAbs = pparent->getSizeAbs();
    float x = computeCoord(pos.x, 0, parentSizeAbs.x, sizeAbsCache.x);
    float y = computeCoord(pos.y, 0, parentSizeAbs.y, sizeAbsCache.y);
    pos.x = x;
    pos.y = y;
  }
  this->posRelative = pos;
  updatePosAbs();
}

const sf::Vector2f Element::getPosAbs() const {
  return posAbsCache;
}

void Element::setPosAbs(const sf::Vector2f pos) {
  this->posAbsCache = pos;
}

const sf::Vector2f Element::getSizeAbs() const {
  return sizeAbsCache;
}

void Element::setSizeAbs(const sf::Vector2f size) {
  this->sizeAbsCache = size;
}

const Relatif2 Element::getSizeRelative() const {
  return sizeRelative;
}

void Element::setSizeRelative(const Relatif2 sizeRelative) {
  this->sizeRelative = sizeRelative;
  updateSizeAbs();
}

void Element::add(Element* pchild) {
  pchildren.push_back(pchild);
  pchild->setParent(this);
}

void Element::remove(Element* pchild) {
  for (auto it = pchildren.begin(); it != pchildren.end(); it++) {
    if (*it == pchild) {
      *it = nullptr;
      pchildren.erase(it);
      return;
    }
  }
}

void Element::notifyEditPosAbs() {
  for (auto pchild : pchildren) {
    pchild->reactEditPosAbsParent();
  }
}

void Element::notifyEditSizeAbs() {
  for (auto pchild : pchildren) {
    pchild->reactEditSizeAbsParent();
  }
}

void Element::reactEditPosAbsParent() {
  updatePosAbs();
}

void Element::reactEditSizeAbsParent() {
  updatePosAbs();
  updateSizeAbs();
}

void Element::setParent(Element* pparent) {
  this->pparent = pparent;
  if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
    cout << ">>>edit parent: " << pparent->getId() << "<-" << getId() << endl;
  }
  updateDepth();
  reactEditPosAbsParent();
  reactEditSizeAbsParent();
}

Element* Element::getParent() const {
  return pparent;
}

void Element::updateDepth() {
  if (pparent == nullptr)
    depthCache = 0;
  else
    depthCache = pparent->getDepth() + 1;

  for (auto pchild : pchildren) {
    pchild->updateDepth();
  }
}

unsigned int Element::getDepth() const {
  return depthCache;
}

namespace render {

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vec) {
  os << "(";
  os << std::setw(3) << vec.x;
  os << ", ";
  os << std::setw(3) << vec.y;
  os << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Element& el) {
  auto classname = typeid(el).name();
  auto sep = string(el.getDepth() * 4, ' ');

  os << sep;
  os << classname;
  os << std::setw(3) << el.id;
  if (el.getMouseOver())
    os << " Mouse over";
  os << endl;

  if (el.pparent) {
    os << sep;
    os << "parent abs:  ";
    os << el.pparent->getPosAbs();
    os << " ";
    os << el.pparent->getSizeAbs();
    os << endl;

    os << sep;
    os << "*this rel:   ";
    os << el.getPosRelative();
    os << " ";
    os << el.getSizeRelative();
    os << endl;
  }

  os << sep;
  os << "*this abs:   ";
  os << el.getPosAbs();
  os << " ";
  os << el.getSizeAbs();
  os << endl;
  return os;
}
}  // namespace render

std::string Element::getTreeView() const {
  std::stringstream stream;
  stream << *this;

  if (!pchildren.empty())
    stream << endl;

  size_t count = 0;
  for (auto pchild : pchildren) {
    stream << pchild->getTreeView();
    count++;
    if (count != pchildren.size())
      stream << endl;
  }
  if (depthCache == 0)
    stream << endl;

  return stream.str();
}

void Element::updatePosAbs() {
  sf::Vector2f parentPosAbs, parentSizeAbs;
  parentPosAbs = pparent ? pparent->getPosAbs() : sf::Vector2f{0, 0};
  parentSizeAbs = pparent ? pparent->getSizeAbs() : sf::Vector2f{0, 0};
  float x = computeCoord(posRelative.x, parentPosAbs.x, parentSizeAbs.x,
                         sizeAbsCache.x);
  float y = computeCoord(posRelative.y, parentPosAbs.y, parentSizeAbs.y,
                         sizeAbsCache.y);
  sf::Vector2f posAbsNew = {x, y};

  if (posAbsCache != posAbsNew) {
    posAbsCache = posAbsNew;
    if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
      cout << ">>>updatePosAbs " << id << endl;
      cout << (*this) << endl;
    }
    notifyEditPosAbs();
  }
}

void Element::updateSizeAbs() {
  sf::Vector2f parentSizeAbs;
  parentSizeAbs = pparent ? pparent->getSizeAbs() : sf::Vector2f{0, 0};
  float x = computeLength(sizeRelative.x, parentSizeAbs.x);
  float y = computeLength(sizeRelative.y, parentSizeAbs.y);
  sf::Vector2f sizeAbsNew = {x, y};

  if (sizeAbsCache != sizeAbsNew) {
    sizeAbsCache = sizeAbsNew;
    if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG)) {
      cout << ">>>updateSizeAbs " << id << endl;
      cout << (*this) << endl;
    }
    notifyEditSizeAbs();
    updatePosAbs();
  }
}

float Element::computeCoord(Relatif rel,
                            float parentCoordAbs,
                            float parentLengthAbs,
                            float lengthAbs) {
  float f = parentCoordAbs;
  float pixel = rel.getPixel();
  switch (rel.getComputeMethod()) {
    case ComputeMethodType::pixel: {
      f += pixel;
      if (pparent) {
        float flimit = parentCoordAbs + parentLengthAbs - lengthAbs;
        if (DEBUGING && (IDDEBUG == -1 || getId() == IDDEBUG))
          cout << "flimit = " << flimit << " = " << parentCoordAbs << " + "
               << parentLengthAbs << "-" << lengthAbs << endl;

        f += (pixel < 0 ? parentLengthAbs - lengthAbs : 0);
        f = f > flimit ? flimit : f;
      }
      f = f < 0 ? 0 : f;
      break;
    }

    case ComputeMethodType::percent:
      if (!pparent)
        f += pixel >= 0 ? pixel : 0;
      else {
        auto percent = rel.getPercent();
        f += percent / 100 * parentLengthAbs;
        f += (percent < 0 ? parentLengthAbs - lengthAbs : 0);
      }
      break;

    case ComputeMethodType::alignement: {
      if (!pparent)
        break;

      auto ali = rel.getAlignement();
      if (ali == "t" || ali == "l")
        break;
      else if (ali == "b" || ali == "r")
        f += parentLengthAbs - lengthAbs;
      else if (ali == "m")
        f += parentLengthAbs / 2 - lengthAbs / 2;
      else if (ali == "q")
        f += parentLengthAbs / 4 - lengthAbs / 2;
      else if (ali == "3q")
        f += parentLengthAbs * 3 / 4 - lengthAbs / 2;
      break;
    }

    default:
      break;
  }
  return f;
}

float Element::computeLength(Relatif rel, float parentLengthAbs) {
  float f;
  switch (rel.getComputeMethod()) {
    case ComputeMethodType::pixel: {
      float pixel = rel.getPixel();
      if (!pparent)
        f = pixel >= 0 ? pixel : 0;
      else
        f = pixel + (pixel < 0 ? parentLengthAbs : 0);
      break;
    }

    case ComputeMethodType::percent: {
      if (pparent) {
        f = parentLengthAbs * rel.getPercent() / 100;
        f += (percent < 0 ? parentLengthAbs : 0);
      } else
        f = 0;
      break;
    }

    default:
      f = 0;
      break;
  }
  return f;
}

bool Element::isInside(sf::Vector2f posAbs) {
  return posAbsCache.x <= posAbs.x &&
         posAbs.x <= posAbsCache.x + sizeAbsCache.x &&
         posAbsCache.y <= posAbs.y &&
         posAbs.y <= posAbsCache.y + sizeAbsCache.y;
}

int Element::getId() const {
  return id;
}

bool Element::getWaitingDestruction() const {
  return waitingDestruction;
}

void Element::setWaitingDestruction(bool b) {
  waitingDestruction = b;
}

void Element::moveAtTop(Element* pchild) {
  auto it = std::find(pchildren.begin(), pchildren.end(), pchild);
  if (it == pchildren.end())
    return;
  rotate(pchildren.begin(), it, it + 1);
}

void Element::setMouseOver(bool b, sf::Vector2f mousePosAbs) {
  this->mouseOverCache = b;
  updateChildrenMouseOver(mousePosAbs);
}

bool Element::getMouseInChildren(sf::Vector2f mousePosAbs) const {
  for (auto pchild : pchildren) {
    if (pchild->isInside(mousePosAbs))
      return true;
  }
  return false;
}

void Element::processEvent(sf::Event event, sf::Vector2f mousePosAbs) {
  if (link != nullptr && this->getMouseOver()) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
      Element* root = this->getParent();
      Element* parent = nullptr;
      while (root->getParent() != nullptr) {
        parent = root;
        root = root->getParent();
      }
      if (!root->getChild(link)) {
        auto pos = parent->getPosRelative();
        auto size = parent->getSizeRelative();
        link->setPosRelative(
            {pos.x.getPixel() + size.x.getPixel(), pos.y.getPixel()});
        root->add(link);
      }
    }
  }
}

bool Element::getMouseOver() const {
  return mouseOverCache;
}

std::vector<Element*> Element::getChildren() const {
  return pchildren;
}

void Element::setChildren(vector<Element*> pchildren) {
  this->pchildren = pchildren;
}

Element* Element::getChild(Element* child) {
  for (auto children : pchildren)
    if (children == child)
      return child;
  return nullptr;
}

void Element::setLink(Element* link) {
  this->link = link;
}

Element* Element::getLink() {
  return link;
}

vector<Element*> Element::getLinked() {
  vector<Element*> v;
  if (this) {
    if (this->getLink())
      v.push_back(this);
    for (auto child : this->getChildren()) {
      vector<Element*> v2 = child->getLinked();
      if (v2.size())
        v.insert(v.end(), v2.begin(), v2.end());
    }
  }
  return v;
}

vector<Element*> Element::getLinks() {
  vector<Element*> v;
  if (this) {
    if (this->getLink())
      v.push_back(link);
    for (auto child : this->getChildren()) {
      vector<Element*> v2 = child->getLinks();
      if (v2.size())
        v.insert(v.end(), v2.begin(), v2.end());
    }
  }
  return v;
}