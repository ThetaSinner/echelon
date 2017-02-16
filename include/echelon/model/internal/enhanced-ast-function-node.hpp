#ifndef ENHANCED_AST_FUNCTION_NODE_HPP_INCLUDED
#define ENHANCED_AST_FUNCTION_NODE_HPP_INCLUDED

#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-context-aware-node.hpp>

class EnhancedAstFunctionNode : public EnhancedAstNode, public EnhancedAstContextAwareNode {
  EnhancedAstNode* impl = nullptr;

public:
  void setImpl(EnhancedAstNode* impl) {
    this->impl = impl;
  }

  EnhancedAstNode* getImpl() {
    return impl;
  }
};

#endif
