#include "Inventory.h"

using namespace std;
using namespace state;

Inventory::Inventory() {}

void Inventory::addItem(Item* item) {
  content.push_back(item);
}

void Inventory::delItem(Item* item) {
  for (auto it = content.begin(); it != content.end(); it++) {
    if (*it == item)
      content.erase(it);
  }
}
