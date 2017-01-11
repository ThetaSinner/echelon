#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>

void AstEnhancerHelper::mapBlockIfPresent(AstNode* nodeToMap, EnhancedAstNode* target, const AstNodeEnhancerInputData& input) {
  if (nodeToMap->hasChild(AstNodeType::Block)) {
    AstNodeEnhancerInputData subInput = input;
    subInput.setTargetNode(target);
    subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::Block));

    NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Block)(subInput);
  }
}