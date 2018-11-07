#include "Window.h"
#include <iostream>
#include "Rectangle.h"

using namespace render;
using namespace std;

Window::Window() {
  cout << "window init" << endl;
  posParent = sf::Vector2f(0, 0);
  setSize({100, 100});
  ptitleBar = new TitleBar();
  pcontent = new Rectangle();

  add(ptitleBar);
  add(pcontent);
}