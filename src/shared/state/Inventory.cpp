#include "Inventory.h"
#include "Item.h"

state::Inventory::Inventory() {}

namespace state {

void Inventory::addItem(Item& item) {
  // TODO
}

void Inventory::delItem(std::size_t i) {
  if (i >= content.size()) {
    // TODO: trhow error
  }
  content.erase(content.begin() + i);
}

}  // namespace state