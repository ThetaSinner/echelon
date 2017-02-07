#ifndef SCOPE_HELPER_HPP_INCLUDED
#define SCOPE_HELPER_HPP_INCLUDED

class Scope;

#include <echelon/ast/transform-stage/scope.hpp>

class ScopeHelper {
public:
  static Scope* createChildScope(Scope* scope);
  static void linkScopes(Scope* target, Scope* toBeLinked);
};

#endif
