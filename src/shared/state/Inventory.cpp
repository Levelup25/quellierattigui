#include "Inventory.h"
#include "Item.h"

state::Inventory::Inventory() {}

namespace state {

void Inventory::addItem(Item& item) {
  // TODO
}

void Inventory::delItem(Item* pItem) {
  for (auto it = content.begin(); it != content.end(); it++) {
    Item* pi = *it;
    if (pi == pItem) {
      content.erase(it);
      return;
    }
  }
}

}  // namespace state