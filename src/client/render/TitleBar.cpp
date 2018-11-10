#include "TitleBar.h"
#include <iostream>
#include "TitleBarButton.h"

using namespace render;
using namespace std;

TitleBar::TitleBar() {
  recshape.setFillColor(sf::Color::Blue);
  setSizeRelative({(string) "100%", 30});

  pcloseBtn = new TitleBarButton();
  pcloseBtn->setPosRelative({-5, -5});
  add(pcloseBtn);
}