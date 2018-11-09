#include "Window.h"
#include <iostream>
#include "Rectangle.h"

using namespace render;
using namespace std;

Window::Window() {
  ptitleBar = new TitleBar();
  // pcontent = new Rectangle();
  add(ptitleBar);
  // add(pcontent);
  setSizeRelative({100, 100});
}