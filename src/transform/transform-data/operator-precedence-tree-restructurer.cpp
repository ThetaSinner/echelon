#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>

EnhancedAstNode* OperatorPrecedenceTreeRestructurer::restructure(EnhancedAstNode *node) {
  auto operatorSubType = EnhancedAstNodeSubType::First;
  while ((operatorSubType = nextOperator(operatorSubType)) != EnhancedAstNodeSubType::Last) {

    // Find the first operator of this type.
    EnhancedAstNode *parent = nullptr;
    auto oper = node;
    while (oper->getNodeSubType() != operatorSubType && oper->getChildCount() > 0) {
      parent = oper;
      oper = oper->getChild(1);
    }

    if (oper->getNodeType() != EnhancedAstNodeType::BinaryOperator) {
      // We've hit a value, there are no more operators of teh current type, skip to the next type.
      continue;
    }

    if (parent != nullptr) {
      // rotate around this first operator.

      // extract the operator node from its parent.
      parent->removeChild(oper);

      // Move the value from this operator node to the one above. i.e. associate left to the higher precedence operator.
      auto value = oper->getChild(0);
      oper->removeChild(value);
      parent->putChild(value);

      // rotate the left part.
      auto leftPart = restructure(node);
      oper->putChild(leftPart);

      // rotate the right part.
      auto rightPart = oper->getChild(0);
      oper->removeChild(rightPart);
      oper->putChild(restructure(rightPart));

      return oper;
    }
  }

  return node;
}

EnhancedAstNodeSubType OperatorPrecedenceTreeRestructurer::nextOperator(EnhancedAstNodeSubType astNodeType) {
  switch (astNodeType) {
    case EnhancedAstNodeSubType::First:
      return EnhancedAstNodeSubType::Subtract;
    case EnhancedAstNodeSubType::Subtract:
      return EnhancedAstNodeSubType::Add;
    default:
      return EnhancedAstNodeSubType::Last;
  }
}