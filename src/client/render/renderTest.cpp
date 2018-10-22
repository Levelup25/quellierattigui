#include "renderTest.h"
#include "IGWindow.h"

void testRender() {
  // resources
  sf::Color cBlue = sf::Color(42, 103, 146);
  sf::Color white = sf::Color(255, 255, 255);
  sf::Color black = sf::Color(0, 0, 0);
  sf::Color grey = sf::Color(100, 100, 100);
  sf::Color snow = sf::Color(240, 240, 240);

  sf::Font roboto;
  roboto.loadFromFile(
      "/home/louis/Documents/3A-IS/quellierattigui/res/font/roboto/"
      "Roboto-Regular.ttf");

  sf::RenderWindow window(sf::VideoMode(700, 700), "Test rendu");

  // Custom windows test
  IGWindow wTest1;
  wTest1.setPosition(sf::Vector2f(10, 350));
  wTest1.setTitle("Test 1");

  IGWindow wTest2;
  wTest2.setPosition(sf::Vector2f(10 + 10 + 100, 350));
  wTest2.setTitle("Test 2");

  // Inventory properties
  sf::Vector2f posInv = sf::Vector2f(10, 10);
  size_t nbRow = 10, nbCol = 30;
  float headerHeight = 30;
  float footerHeight = 20;
  float bodyPadding = 10;
  float caseSize = 16;
  float barBtnSize = 20;

  // calcultated properties of inventory
  sf::Vector2f posBody2Inv =
      sf::Vector2f(bodyPadding, bodyPadding + headerHeight);
  float invHeight, invWidth;
  invWidth = bodyPadding + nbCol * caseSize + bodyPadding;
  invHeight = headerHeight + bodyPadding + nbRow * caseSize + bodyPadding +
              footerHeight;
  int nbItems = nbCol * nbRow;

  // ============== Create window elements ================
  // Inventory windows
  sf::RectangleShape rec = sf::RectangleShape();
  rec.setPosition(posInv);
  rec.setSize(sf::Vector2f(invWidth, invHeight));
  rec.setFillColor(cBlue);

  // window header
  sf::RectangleShape recHeader = sf::RectangleShape();
  recHeader.setPosition(posInv);
  recHeader.setSize(sf::Vector2f(invWidth, headerHeight));
  recHeader.setFillColor(snow);

  // Window title
  sf::Text title;
  title.setFont(roboto);
  title.setString("Inventaire");
  title.setPosition(posInv + sf::Vector2f(bodyPadding, 0));
  title.setCharacterSize(20);
  title.setFillColor(black);

  // Close btn
  sf::RectangleShape closeBtn = sf::RectangleShape();
  sf::Vector2f posCloseBtn2inv =
      sf::Vector2f(invWidth - bodyPadding - barBtnSize, 5);
  closeBtn.setPosition(posInv + posCloseBtn2inv);
  closeBtn.setSize(sf::Vector2f(barBtnSize, barBtnSize));
  closeBtn.setFillColor(sf::Color::Red);

  // Invetory cases
  sf::RectangleShape cases[nbRow][nbCol];
  for (size_t i = 0; i < nbRow; i++) {
    for (size_t j = 0; j < nbCol; j++) {
      sf::RectangleShape invCase;
      invCase.setSize(sf::Vector2f(caseSize, caseSize));
      sf::Vector2f posCase2Body = sf::Vector2f(j * caseSize, i * caseSize);
      invCase.setPosition(posInv + posBody2Inv + posCase2Body);
      invCase.setFillColor(white);
      invCase.setOutlineThickness(1);
      invCase.setOutlineColor(grey);
      cases[i][j] = invCase;
    }
  }

  // Footer messages
  sf::Text footerMsg;
  footerMsg.setFont(roboto);
  footerMsg.setColor(snow);
  footerMsg.setString("Items 1 - " + std::to_string(nbItems));
  footerMsg.setCharacterSize(15);
  sf::Vector2f posFooterMsg2Body =
      sf::Vector2f(bodyPadding, bodyPadding + nbRow * caseSize + bodyPadding);
  footerMsg.setPosition(posBody2Inv + posFooterMsg2Body);

  // ================== window loop ==================
  bool closeBtnPressed = false;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
          sf::Vector2f mousePos = sf::Vector2f(mousePosInt.x, mousePosInt.y);
          if (posInRec(mousePos, closeBtn))
            closeBtnPressed = true;
        }
      }

      else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
          sf::Vector2f mousePos = sf::Vector2f(mousePosInt.x, mousePosInt.y);
          if (posInRec(mousePos, closeBtn))
            if (closeBtnPressed)
              window.close();
        }
      }
    }

    window.clear();

    window.draw(wTest1);
    window.draw(wTest2);
    window.draw(rec);
    window.draw(recHeader);
    window.draw(closeBtn);
    window.draw(title);
    for (size_t i = 0; i < nbRow; i++) {
      for (size_t j = 0; j < nbCol; j++) {
        window.draw(cases[i][j]);
      }
    }
    window.draw(footerMsg);

    window.display();
  }
}

// ================== functions ===============================
bool posInRec(sf::Vector2f pos, sf::RectangleShape rec) {
  sf::Vector2f recPos = rec.getPosition();
  sf::Vector2f recSize = rec.getSize();

  if (recPos.x <= pos.x && pos.x <= (recPos.x + recSize.x) &&
      recPos.y <= pos.y && pos.y <= (recPos.y + recSize.y)) {
    return true;
  } else {
    return false;
  }
}