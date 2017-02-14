#include <echelon/model/internal/enhanced-ast-block-node.hpp>

void EnhancedAstBlockNode::setScope(Scope* scope) {
  this->scope = scope;
}

Scope* EnhancedAstBlockNode::getScope() {
  return scope;
}