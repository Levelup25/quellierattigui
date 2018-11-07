#include "TitleBar.h"
#include <iostream>

using namespace render;
using namespace std;

TitleBar::TitleBar() {
  cout << "titleBar init" << endl;

  auto borderRecshape = pborder->getRecshape();
  borderRecshape.setFillColor(sf::Color::Blue);
  pborder->setRecshape(borderRecshape);
}