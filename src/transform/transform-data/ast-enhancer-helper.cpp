#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>

void AstEnhancerHelper::mapBlockIfPresent(AstNode* nodeToMap, EnhancedAstNode* target, const AstNodeEnhancerInputData& input) {
  if (nodeToMap->hasChild(AstNodeType::Block)) {
    AstNodeEnhancerInputData subInput = input;
    subInput.setTargetNode(target);
    subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::Block));

    // Pass the updated context to the block we're about to map.
    auto contextAware = AstEnhancerHelper::getContextAwareNode(target);
    if (contextAware != nullptr) {
      subInput.setUpdatedContext(contextAware->getContext());
    }

    NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Block)(subInput);
  }
}

void AstEnhancerHelper::simpleMapNode(AstNode* nodeToMap, EnhancedAstNode* target, AstNodeEnhancerInputData& input) {
  auto subInput = input;
  subInput.setTargetNode(target);
  subInput.setNodeToMap(nodeToMap);
  NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getType())(subInput);
}

// TODO This was a bit too simplistic, and can probably be removed.. could still be useful.
std::string AstEnhancerHelper::computeFunctionHash(EnhancedAstNode* enhancedAstNode) {
  std::stringstream ss;

  auto working_name_structure_node = enhancedAstNode;
  while (working_name_structure_node->hasChild(EnhancedAstNodeType::NameStructure)) {
    ss << working_name_structure_node->getData() << "-";
    working_name_structure_node = enhancedAstNode->getChild(EnhancedAstNodeType::NameStructure);
  }

  // the function name
  ss << enhancedAstNode->getData();

  if (enhancedAstNode->hasChild(EnhancedAstNodeType::FunctionParamDefinitions)) {
    ss << "-";

    auto paramDefinitions = enhancedAstNode->getChild(EnhancedAstNodeType::FunctionParamDefinitions);
    int childCount = paramDefinitions->getChildCount();
    for (unsigned i = 0; i < childCount; i++) {
      if (paramDefinitions->getChild(i)->hasChild(EnhancedAstNodeType::TypeName)) {
        ss << paramDefinitions->getChild(i)->getChild(EnhancedAstNodeType::TypeName)->getData() << "-";
      }

      ss << paramDefinitions->getChild(i)->getData();
      if (i != childCount) {
        ss << "-";
      }
    }
  }

  return ss.str();
}

bool AstEnhancerHelper::doFunctionSignaturesMatch(EnhancedAstNode *left, EnhancedAstNode *right) {
#ifdef ECHELON_DEBUG
  if (left->getNodeType() != EnhancedAstNodeType::Function || right->getNodeType() != EnhancedAstNodeType::Function) {
    throw std::runtime_error("doFunctionSignaturesMatch expects both nodes to have type function.");
  }
#endif

  // Handle functions with empty signatures.
  if (!left->hasChild(EnhancedAstNodeType::FunctionParamDefinitions)) {
    return !right->hasChild(EnhancedAstNodeType::FunctionParamDefinitions);
  }
  else if (!right->hasChild(EnhancedAstNodeType::FunctionParamDefinitions)) {
    return false;
  }

  auto leftParams = left->getChild(EnhancedAstNodeType::FunctionParamDefinitions);
  auto rightParams = right->getChild(EnhancedAstNodeType::FunctionParamDefinitions);

  auto paramCount = leftParams->getChildCount();

  // If the two functions have a different number of arguments then they can't match.
  if (rightParams->getChildCount() != paramCount) {
    return false;
  }

  // Look for any differences, if one is found the set match to false.
  bool match = true;
  for (unsigned i = 0; i < leftParams->getChildCount(); i++) {
    std::string *leftType = nullptr;
    if (leftParams->getChild(i)->getChildCount()) {
      leftType = &leftParams->getChild(i)->getChild(0)->getData();
    }

    std::string *rightType = nullptr;
    if (rightParams->getChild(i)->getChildCount()) {
      rightType = &rightParams->getChild(i)->getChild(0)->getData();
    }

    // If one type is specified and the other is not, or both types are specified and they are different then there is a
    // difference between the signatures and we can stop looking.
    if (leftType == nullptr && rightType != nullptr) {
      match = false;
      break;
    } else if (leftType != nullptr && rightType == nullptr) {
      match = false;
      break;
    } else if (leftType != nullptr && *leftType != *rightType) {
      match = false;
      break;
    }
  }

  return match;
}

void AstEnhancerHelper::mapChildIfPresent(EnhancedAstNode* target, AstNode* nodeToMap, const AstNodeEnhancerInputData& input, AstNodeType astNodeType) {
  if (nodeToMap->hasChild(astNodeType)) {
    auto subInput = input;
    subInput.setTargetNode(target);
    subInput.setNodeToMap(nodeToMap->getChild(astNodeType));

    NodeEnhancerLookup::getInstance()->getNodeEnhancer(astNodeType)(subInput);
  }
}

EnhancedAstContextAwareNode* AstEnhancerHelper::getContextAwareNode(EnhancedAstNode* node) {
  switch (node->getNodeType()) {
    case EnhancedAstNodeType::Variable:
      {
        auto variable = (EnhancedAstVariableNode*) node;
        return (EnhancedAstContextAwareNode*) variable;
      }
    case EnhancedAstNodeType::Function:
      {
        auto function = (EnhancedAstFunctionNode*) node;
        return (EnhancedAstContextAwareNode*) function;
      }
    case EnhancedAstNodeType::Module:
      {
        auto module = (EnhancedAstModuleNode*) node;
        return (EnhancedAstContextAwareNode*) module;
      }
    case EnhancedAstNodeType::CustomType:
      {
        auto customType = (EnhancedAstCustomTypeNode*) node;
        return (EnhancedAstContextAwareNode*) customType;
      }
    default:
      break;
  }

  return nullptr;
}