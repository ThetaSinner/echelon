#include <echelon/transform/scope-helper.hpp>

Scope* ScopeHelper::createChildScope(Scope* scope) {
  Scope *newScope = new Scope();
  newScope->setParentScope(scope);
  return newScope;
}

void ScopeHelper::linkScopes(Scope* target, Scope* toBeLinked) {
  // Only linker to the target scope, don't inherit any parents of the linker.
  toBeLinked->setParentScope(nullptr);

  target->pushLinkedScope(toBeLinked);
}