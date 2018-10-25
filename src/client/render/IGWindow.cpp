#include "IGWindow.h"

void IGWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sfBorder);
  target.draw(sfHeader);
  target.draw(sfTitle);
  target.draw(sfCloseBtn);
  target.draw(sfContent);
}

IGWindow::IGWindow() {
  roboto.loadFromFile(
      "/home/louis/Documents/3A-IS/quellierattigui/res/font/roboto/"
      "Roboto-Regular.ttf");

  pos = sf::Vector2f(10, 10);
  size = sf::Vector2f(100, 100);

  // Border
  sfBorder.setPosition(pos);
  sfBorder.setSize(size);
  sfBorder.setOutlineThickness(1);
  sfBorder.setOutlineColor(uicolor::black);
  sfBorder.setFillColor(sf::Color::Transparent);

  // Content
  sfContent.setPosition(pos + sf::Vector2f(0, headerHeight));
  sfContent.setSize(size - sf::Vector2f(0, headerHeight));
  sfContent.setFillColor(uicolor::cBlue);

  // Header
  sfHeader.setPosition(pos);
  sfHeader.setSize(sf::Vector2f(size.x, headerHeight));
  sfHeader.setFillColor(uicolor::snow);

  // Title
  sfTitle.setPosition(pos + sf::Vector2f(barPad, 0));
  sfTitle.setFont(roboto);
  sfTitle.setString(title);
  sfTitle.setCharacterSize(20);
  sfTitle.setColor(uicolor::black);

  // Close btn
  sf::Vector2f posRCloseBtn = sf::Vector2f(size.x - barPad - barBtnSize, 5);
  sfCloseBtn.setPosition(pos + posRCloseBtn);
  sfCloseBtn.setSize(sf::Vector2f(barBtnSize, barBtnSize));
  sfCloseBtn.setFillColor(sf::Color::Red);
}

void IGWindow::setPosition(sf::Vector2f newPos) {
  pos = newPos;
  sfBorder.setPosition(pos);
  sfContent.setPosition(pos + sf::Vector2f(0, headerHeight));
  sfHeader.setPosition(pos);
  sfTitle.setPosition(pos + sf::Vector2f(barPad, 0));
  sf::Vector2f posRCloseBtn = sf::Vector2f(size.x - barPad - barBtnSize, 5);
  sfCloseBtn.setPosition(pos + posRCloseBtn);
}

sf::Vector2f IGWindow::getPosition() {
  return pos;
}

void IGWindow::setSize(sf::Vector2f newSize) {
  size = newSize;
  sfBorder.setSize(size);
  sfContent.setSize(size - sf::Vector2f(0, headerHeight));
  sfHeader.setSize(sf::Vector2f(size.x, headerHeight));
  sf::Vector2f posRCloseBtn = sf::Vector2f(size.x - barPad - barBtnSize, 5);
  sfCloseBtn.setPosition(pos + posRCloseBtn);
}

void IGWindow::setSizeContent(sf::Vector2f newSize) {
  setSize(newSize + sf::Vector2f(0, headerHeight));
}

sf::Vector2f IGWindow::getSize() {
  return size;
}

void IGWindow::setTitle(std::string newTitle) {
  title = newTitle;
  sfTitle.setString(title);
}

sf::String IGWindow::getTitle() {
  return title;
}

bool IGWindow::posInRec(sf::Vector2f pos, sf::RectangleShape rec) {
  sf::Vector2f recPos = rec.getPosition();
  sf::Vector2f recSize = rec.getSize();

  if (recPos.x <= pos.x && pos.x <= (recPos.x + recSize.x) &&
      recPos.y <= pos.y && pos.y <= (recPos.y + recSize.y)) {
    return true;
  } else {
    return false;
  }
}

void IGWindow::receiveEvent(sf::Event event, sf::Vector2f posMouse) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      posLastPressed = posMouse;

      if (posInRec(posLastPressed, sfHeader) ||
          posInRec(posLastPressed, sfContent)) {
        displayFirst = true;
      }

      if (posInRec(posLastPressed, sfHeader) &&
          !posInRec(posLastPressed, sfCloseBtn)) {
        offsetMouse = posMouse - pos;
        isDragging = true;
      }
    }
  }

  else if (event.type == sf::Event::MouseButtonReleased) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      posLastRealeased = posMouse;
      isDragging = false;

      if (posInRec(posLastPressed, sfCloseBtn) &&
          posInRec(posLastRealeased, sfCloseBtn)) {
        startClosing();
      }
    }
  }

  else if (event.type == sf::Event::MouseMoved) {
    if (isDragging) {
      auto newPos = posMouse - offsetMouse;
      setPosition(newPos);
    } else {
      offsetMouse = {0, 0};
    }
  }
}

void IGWindow::startClosing() {
  std::string s;
  s = title;
  isClosing = true;
}

IGWindow::~IGWindow() {}