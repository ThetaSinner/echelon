#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>

EnhancedAstNode* OperatorPrecedenceTreeRestructurer::restructureInternal(EnhancedAstNode* node, EnhancedAstNodeSubType nodeSubType) {
  // TODO needs to allow integer float... etc so change to check for children.
  if (node->getNodeType() == EnhancedAstNodeType::VariableValue) {
    return node;
  }
  if (node->getNodeType() == EnhancedAstNodeType::ExpressionGroup) {
    auto subExpression = node->getChild(0);
    node->removeChild(subExpression);
    node->putChild(restructureInternal(subExpression, EnhancedAstNodeSubType::First));
    return node;
  }

  auto newRootNode = node; // alternate return value if the root node changes.
  nodeSubType = nextOperator(nodeSubType);

  std::list<std::pair<EnhancedAstNode*, EnhancedAstNode*>> operList;

  EnhancedAstNode *parent = nullptr;
  auto oper = node;
  while (oper->getChildCount() > 0) {
    if (oper->getNodeType() == EnhancedAstNodeType::BinaryOperator && oper->getNodeSubType() == nodeSubType) {
      operList.push_back(std::make_pair(oper, parent));
    }
    parent = oper;
    if (oper->hasChild(EnhancedAstNodeType::BinaryOperator)) {
      oper = oper->getChild(1);
    }
    else {
      break;
    }
  }

  if (operList.empty()) {
    return restructureInternal(node, nodeSubType);
  }

  for (auto& i : operList) {
    auto processLeft = i.first->getChild(0);
    i.first->removeChild(processLeft);
    i.first->putChildFront(restructureInternal(processLeft, EnhancedAstNodeSubType::First));
  }

  auto firstOperator = operList.front().first;
  auto firstParent = operList.front().second;

  if (operList.front().second != nullptr) {
    firstParent->removeChild(firstOperator);

    auto leftTerm = firstOperator->getChild(0);
    firstOperator->removeChild(leftTerm);

    firstParent->putChild(leftTerm);

    firstOperator->putChildFront(restructureInternal(newRootNode, nodeSubType));

    newRootNode = firstOperator;
  }

  auto iter = operList.begin();
  while (iter != operList.end() && std::next(iter, 1) != operList.end()) {
    auto highOperator = iter->first;

    auto lowOperator = std::next(iter, 1)->first;
    auto lowParent = std::next(iter, 1)->second;

    // There are two operators of the same type adjacent to one another, no need to process what's between them.
    if (highOperator == lowParent) {
      iter++;
      continue;
    }

    auto extractRoot = highOperator->getChild(1);

    // TODO check for two of the same operator adjacent.

    // break top link.
    highOperator->removeChild(extractRoot);

    // break bottom link.
    lowParent->removeChild(lowOperator);

    // grab the value from the bottom operator to associate it to the tree being pushed down.
    auto leftTerm = lowOperator->getChild(0);
    lowOperator->removeChild(leftTerm);

    // Attach the top level to the bottom level to cover the break.
    highOperator->putChild(lowOperator);

    // Attach the value that was grabbed off the top level of the tree.
    lowParent->putChild(leftTerm);

    // Inject the part we are pushing down after restructuring it.
    lowOperator->putChildFront(restructureInternal(extractRoot, nodeSubType));

    iter++;
  }

  auto lastOperator = operList.back().first;

  // Restructure everything after the last operator of this type.
  if (lastOperator->getChildCount() > 0 && lastOperator->getChild(1)->getNodeType() == EnhancedAstNodeType::BinaryOperator) {
    auto extractRoot = lastOperator->getChild(1);
    lastOperator->removeChild(extractRoot);
    lastOperator->putChild(restructureInternal(extractRoot, nodeSubType));
  }
  else if (lastOperator->getChildCount() > 0 && lastOperator->getChild(1)->getNodeType() == EnhancedAstNodeType::ExpressionGroup) {
    auto extractRoot = lastOperator->getChild(1)->getChild(0);
    lastOperator->getChild(1)->removeChild(extractRoot);
    lastOperator->getChild(1)->putChild(restructureInternal(extractRoot, EnhancedAstNodeSubType::First));
  }

  return newRootNode;
}

EnhancedAstNode* OperatorPrecedenceTreeRestructurer::restructure(EnhancedAstNode* node) {
  return restructureInternal(node, EnhancedAstNodeSubType::First);
}

EnhancedAstNodeSubType OperatorPrecedenceTreeRestructurer::nextOperator(EnhancedAstNodeSubType astNodeType) {
  switch (astNodeType) {
    case EnhancedAstNodeSubType::First:
      return EnhancedAstNodeSubType::Subtract;
    case EnhancedAstNodeSubType::Subtract:
      return EnhancedAstNodeSubType::Add;
    case EnhancedAstNodeSubType::Add:
      return EnhancedAstNodeSubType::Multiply;
    case EnhancedAstNodeSubType::Multiply:
      return EnhancedAstNodeSubType::Divide;
    default:
      return EnhancedAstNodeSubType::Last;
  }
}