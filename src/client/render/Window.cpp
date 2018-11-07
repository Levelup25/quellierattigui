#include "Window.h"
#include "Rectangle.h"

using namespace render;

Window::Window() {
  posParent = sf::Vector2f(0, 0);
  size = sf::Vector2f(100, 100);
  pborder = new Rectangle();
  ptitleBar = new TitleBar();
  pcontent = new Rectangle();

  add(pborder);
  add(ptitleBar);
  add(pcontent);

  auto borderRecshape = pborder->getRecshape();
  borderRecshape.setSize(size);
  borderRecshape.setOutlineThickness(1);
  borderRecshape.setOutlineColor(sf::Color::Black);
  borderRecshape.setFillColor(sf::Color::White);
  borderRecshape.setPosition(posParent);
  pborder->setRecshape(borderRecshape);
}