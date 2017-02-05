#include <echelon/ast/transform-stage/enhanced_ast_function_prototype_node.hpp>

void EnhancedAstFunctionPrototypeNode::setImpl(EnhancedAstNode* impl) {
  this->impl = impl;
}

EnhancedAstNode* EnhancedAstFunctionPrototypeNode::getImpl() {
  return impl;
}