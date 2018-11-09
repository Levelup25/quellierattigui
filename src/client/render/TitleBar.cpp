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

void TitleBar::reactEditSizeParent() {
  auto parentWidth = getParent()->getSizeAbs().x;
  setSizeRelative({parentWidth, 30});
  float closePadR = 5;
  auto closeBtnSize = pcloseBtn->getSizeAbs().x;
  auto closeBtnX = getSizeAbs().x - closeBtnSize - closePadR;
  pcloseBtn->setPosRelative({closeBtnX, 5});
}