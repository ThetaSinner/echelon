#include <echelon/model/scope/scope-helper.hpp>

Scope* ScopeHelper::createChildScope(Scope* scope) {
  Scope *newScope = new Scope();
  newScope->setParentScope(scope);
  return newScope;
}

void ScopeHelper::linkScopes(Scope* target, Scope* toBeLinked) {
  // Only link to the target scope, don't inherit any parents of the link.
  toBeLinked->setParentScope(nullptr);

  target->pushLinkedScope(toBeLinked);
}

void ScopeHelper::makeParentOf(Scope *child, Scope *parent) {
  child->setParentScope(parent);
}

void ScopeHelper::moveTempScopeToScope(Scope *temp, Scope *scope) {
  for (auto pd : temp->getParamDefinitions()) {
    scope->addParamDefinition(pd.first, pd.second);
  }
}