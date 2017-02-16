#include <echelon/model/context/context.hpp>

Context::Context(Context *parentContext, ContextItem* contextItem) {
  this->parentContext = parentContext;
  this->contextItem = contextItem;
}

ContextItem* Context::getContextItem() {
  return contextItem;
}

Context* Context::getParentContext() {
  return parentContext;
}

std::string Context::toString(bool recursive) {
  auto prefix = (recursive ? "" : "cp:/");

  std::string step = "";
  if (parentContext != nullptr) {
    step = parentContext->toString(true) + "/";
  }
  auto end = contextItem->getName();
  return prefix + step + end;
}