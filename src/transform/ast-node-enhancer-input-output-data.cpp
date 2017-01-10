#include <echelon/transform/ast-node-enhancer-output-data.hpp>

AstNodeEnhancerOutputData::AstNodeEnhancerOutputData(AstNodeEnhancerInputData& output) {
  setSourceNode(output.getSourceNode());
  setTargetNode(output.getTargetNode());
}

AstNode* AstNodeEnhancerOutputData::getSourceNode() {
  return source;
}

void AstNodeEnhancerOutputData::setSourceNode(AstNode* source) {
  this ->source = source;
}

EnhancedAstNode* AstNodeEnhancerOutputData::getTargetNode() {
  return target;
}

void AstNodeEnhancerOutputData::setTargetNode(EnhancedAstNode* target) {
  this->target = target;
}
