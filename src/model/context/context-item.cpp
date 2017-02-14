#include <echelon/model/context/context-item.hpp>

ContextItem::ContextItem(std::string name) {
  setName(name);
}

void ContextItem::setName(std::string name) {
  this->name = name;
}

std::string ContextItem::getName() {
  return name;
}