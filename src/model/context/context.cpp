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
  return (recursive ? "" : "cp:/") + (parentContext == nullptr ? "" : parentContext->toString(true) + "/") + contextItem->getName();
}