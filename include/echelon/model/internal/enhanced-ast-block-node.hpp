#ifndef ENHANCED_AST_BLOCK_NODE_HPP_INCLUDED
#define ENHANCED_AST_BLOCK_NODE_HPP_INCLUDED

#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>

class EnhancedAstBlockNode : public EnhancedAstNode {
  Scope* scope;

public:
  void setScope(Scope* scope);
  Scope* getScope();
};

#endif
