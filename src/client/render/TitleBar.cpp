#include "TitleBar.h"
#include <iostream>
#include "TitleBarButton.h"

using namespace render;
using namespace std;

TitleBar::TitleBar() {
  recshape.setFillColor(sf::Color::Blue);
  pcloseBtn = new TitleBarButton();
  add(pcloseBtn);
}

void TitleBar::updateSizeParent() {
  auto parentWidth = getParent()->getSize().x;
  setSize({parentWidth, 30});
  float closePadR = 5;
  auto closeBtnSize = pcloseBtn->getSize().x;
  auto closeBtnX = getSize().x - closeBtnSize - closePadR;
  pcloseBtn->setPosRelative({closeBtnX, 5});
}