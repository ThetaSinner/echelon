#ifndef SCOPE_PUSHER_HPP_INCLUDED
#define SCOPE_PUSHER_HPP_INCLUDED

#include <echelon/model/scope/scope.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>

class ScopePusher {
public:
  // TODO need a context path on the scope, this can be constructed as the scope is built.
  static void push(Scope* scope, EnhancedAstNode* node);
};

#endif
