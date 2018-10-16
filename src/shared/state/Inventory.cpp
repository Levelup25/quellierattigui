#include "Inventory.h"
#include "Item.h"

state::Inventory::Inventory() {}

void state::Inventory::addItem(Item& item) {
  std::unique_ptr pi = &item;
  content.push_back(pi);
}

void state::Inventory::delItem(std::size_t i) {
  if (i >= content.size()) {
    // TODO: trhow error
  }
  content.erase(content.begin() + i);
}