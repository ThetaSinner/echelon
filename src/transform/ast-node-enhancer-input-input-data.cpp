#include <echelon/transform/ast-node-enhancer-input-data.hpp>

AstNode* AstNodeEnhancerInputData::getSourceNode() {
  return source;
}

void AstNodeEnhancerInputData::setSourceNode(AstNode* source) {
  this ->source = source;
}

AstNode* AstNodeEnhancerInputData::getNodeToMap() {
  return nodeToMap;
}

void AstNodeEnhancerInputData::setNodeToMap(AstNode* nodeToMap) {
  this ->nodeToMap = nodeToMap;
}

EnhancedAstNode* AstNodeEnhancerInputData::getTargetNode() {
  return target;
}

void AstNodeEnhancerInputData::setTargetNode(EnhancedAstNode* target) {
  this->target = target;
}

Scope* AstNodeEnhancerInputData::getScope() {
  return scope;
}

void AstNodeEnhancerInputData::setScope(Scope* scope) {
  this->scope = scope;
}