#include <echelon/model/internal/enhanced-ast-function-prototype-node.hpp>

void EnhancedAstFunctionPrototypeNode::setImpl(EnhancedAstNode* impl) {
  this->impl = impl;
}

EnhancedAstNode* EnhancedAstFunctionPrototypeNode::getImpl() {
  return impl;
}
