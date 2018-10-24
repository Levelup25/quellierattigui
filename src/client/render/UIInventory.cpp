#include "UIInventory.h"

UIInventory::UIInventory() {
  float width = contentPadding[2] + nbCol * caseSize.x + contentPadding[3],
        height = contentPadding[0] + nbRow * caseSize.y + vspace +
                 (float)footerTextHight + contentPadding[1];
  setSizeContent(sf::Vector2f{width, height});
  setTitle("Inventory");

  // Invetory cases
  sfcases.resize(getNbItems());
  auto posGrid = getPosGrid();
  for (size_t i = 0; i < nbRow; i++) {
    for (size_t j = 0; j < nbCol; j++) {
      sf::RectangleShape sfcase;
      sfcase.setSize(caseSize);

      sf::Vector2f posCase2Grid = sf::Vector2f(j * caseSize.x, i * caseSize.y);
      sfcase.setPosition(posCase2Grid + posGrid);

      sfcase.setFillColor(uicolor::white);
      sfcase.setOutlineThickness(1);
      sfcase.setOutlineColor(uicolor::grey);

      size_t index = i * nbCol + j;
      sfcases[index] = sfcase;
    }
  }

  // Footer text
  sffooterText.setFont(roboto);
  sffooterText.setFillColor(uicolor::snow);
  sffooterText.setString("Items 1 - " + std::to_string(getNbItems()));
  sffooterText.setCharacterSize(footerTextHight);
  sffooterText.setPosition(getPosFooterText());
}

void UIInventory::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  IGWindow::draw(target, states);
  for (size_t i = 0; i < nbRow; i++) {
    for (size_t j = 0; j < nbCol; j++) {
      size_t index = i * nbCol + j;
      target.draw(sfcases[index]);
    }
  }
  target.draw(sffooterText);
}

// Calculated properties
sf::Vector2f UIInventory::getPosGrid() const {
  auto posContent = sfContent.getPosition();
  sf::Vector2f posGrid{posContent.x + contentPadding[2],
                       posContent.y + contentPadding[0]};
  return posGrid;
}
sf::Vector2f UIInventory::getPosFooterText() const {
  auto posContent = sfContent.getPosition();
  sf::Vector2f posFooterText{
      posContent.x + contentPadding[2],
      posContent.y + contentPadding[0] + caseSize.y * nbRow + vspace};
  return posFooterText;
}

size_t UIInventory::getNbItems() const {
  return nbCol * nbRow;
}

void UIInventory::setPosition(sf::Vector2f newPos) {
  IGWindow::setPosition(newPos);

  auto posGrid = getPosGrid();
  for (size_t i = 0; i < nbRow; i++) {
    for (size_t j = 0; j < nbCol; j++) {
      sf::Vector2f posCase2Grid = sf::Vector2f(j * caseSize.x, i * caseSize.y);
      size_t index = i * nbCol + j;
      sfcases[index].setPosition(posCase2Grid + posGrid);
    }
  }
  sffooterText.setPosition(getPosFooterText());
}