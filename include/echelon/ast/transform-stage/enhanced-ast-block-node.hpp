#ifndef ENHANCED_AST_BLOCK_NODE_HPP_INCLUDED
#define ENHANCED_AST_BLOCK_NODE_HPP_INCLUDED

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/scope/scope.hpp>

class EnhancedAstBlockNode : public EnhancedAstNode {
  Scope* scope;

public:
  void setScope(Scope* scope);
  Scope* getScope();
};

#endif
