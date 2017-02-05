#ifndef ENHANCED_AST_FUNCTION_PROTOTYPE_NODE_INCLUDED
#define ENHANCED_AST_FUNCTION_PROTOTYPE_NODE_INCLUDED

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

class EnhancedAstFunctionPrototypeNode : public EnhancedAstNode {
  EnhancedAstNode* impl = nullptr;

public:
  // TODO how do you use an init list and still split constructor into cpp file?
  EnhancedAstFunctionPrototypeNode(EnhancedAstNode* node) : EnhancedAstNode(*node) {}

  void setImpl(EnhancedAstNode* impl);
  EnhancedAstNode* getImpl();
};

#endif
