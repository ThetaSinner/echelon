#ifndef SCOPE_PUSHER_HPP_INCLUDED
#define SCOPE_PUSHER_HPP_INCLUDED

#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

class ScopePusher {
public:
  // TODO need a context path on the scope, this can be constructed as the scope is built.
  static void push(Scope* scope, EnhancedAstNode* node);
};

#endif
