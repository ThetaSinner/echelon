#include <echelon/transform/ast-enhancer.hpp>

#include <map>
#include <sstream>
#include <functional>
#include <stdexcept>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>
#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>

bool doFunctionSignaturesMatch(EnhancedAstNode *left, EnhancedAstNode *right) {
#ifdef ECHELON_DEBUG
  if (left->getNodeType() != EnhancedAstNodeType::Function || right->getNodeType() != EnhancedAstNodeType::Function) {
    throw std::runtime_error("doFunctionSignaturesMatch expects both nodes to have type function.");
  }
#endif

  // Handle functions with empty signatures.
  if (!left->hasChild(EnhancedAstNodeType::FunctionParamDefinitions)) {
    return !right->hasChild(EnhancedAstNodeType::FunctionParamDefinitions);
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

void enhanceInternal(AstNode *node, EnhancedAstNode *target, Scope* scope) {
  auto working_target = target;

  for (unsigned i = 0; i < node->getChildCount(); i++) {
    NodeEnhancer& nodeEnhancer = NodeEnhancerLookup::getInstance()->getNodeEnhancer(node->getChild(i)->getType());

    AstNodeEnhancerInputData input;
    input.setSourceNode(node);
    input.setNodeToMap(node->getChild(i));
    input.setTargetNode(working_target);
    input.setScope(scope);

    auto output = nodeEnhancer(input);

    // jump to the new working nodes. This allows the enhancer to modify the ast structure.
    working_target = output.getTargetNode();
/*
    if (node->getChild(i)->getType() == AstNodeType::Variable) {
      auto data = node->getChild(i)->getData();
      enhancedNode->setNodeType(EnhancedAstNodeType::Variable);
      enhancedNode->setData(data);

      // This is the first time we've seen this variable in this scope, add it.
      if (!scope.hasVariable(data)) {
        enhancedNode->setNodeSubType(EnhancedAstNodeSubType::Declaration);
        scope.addVariable(data, node->getChild(i));
      }
      else {
        // The variable has been seen before. Check that there is no type declaration.
        if (node->getChild(i)->getChild(0)->getType() == AstNodeType::Type) {
          std::string message = "Error, redeclaration of variable [" + data + "].";
          throw std::runtime_error(message.c_str());
        }

        enhancedNode->setNodeSubType(EnhancedAstNodeSubType::Assign);
      }

      // TODO map variable name and type?

      // TODO this has been removed and should be implemented in the enhancers. mapSubNodes(node->getChild(i), enhancedNode, scope);
    }
    else if (node->getChild(i)->getType() == AstNodeType::Function) {
      // map function and store on scope.
      std::string data = node->getChild(i)->getData();
      // TODO extract to enhancer. enhancedNode = NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Function)(node->getChild(i), scope);

      if (scope.hasFunction(data)) {
        auto functions = scope.getFunctions(data);
        for (auto func : *functions) {
          if (doFunctionSignaturesMatch(func, enhancedNode)) {
            // TODO line+char information.
            std::string message = "Error [" + enhancedNode->getData() + "] re-declares function [" + func->getData() +
                                  "] with indistinguishable signature";
            throw std::runtime_error(message.c_str());
          }
        }
      }

      // None of the existing functions with the same name have the same signature, so it is safe to add it.
      scope.addFunction(data, enhancedNode);
    } else {
      // This is not new data being declared, so map it and ensure that all references to variables and functions are valid.
      // TODO extract to enhancer. target->putChild(NodeEnhancerLookup::getInstance()->getNodeEnhancer(node->getChild(i)->getType())(node->getChild(i), scope));
    }

    target->putChild(enhancedNode);
    */
  }
}

/*void enhanceInternalRoot(AstNode *node, EnhancedAstNode *target, Scope scope) {
  static auto log = LoggerSharedInstance::get();
  
  AstNode *packageNode = nullptr;
  if (node->getChild(0)->getType() == AstNodeType::Package) {
    packageNode = node->getChild(0);
  }
  else if (node->hasChild(AstNodeType::Package)) {
    log->at(Level::Warn) << "Package declaration should be the first statement.";
    packageNode = node->getChild(AstNodeType::Package);
  }

  auto new_node = node;
  auto new_target = target;
  if (packageNode != nullptr) {
    auto& packageEnhancer = NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Package);
    // Build the package structure on the target.
    target->putChild(packageEnhancer(packageNode, scope));

    while (new_target->getChildCount()) {
      new_target = new_target->getChild(EnhancedAstNodeType::Package);
    }

    // Create a new node rather than modifying the exiting node.
    new_node = new AstNode();
    for (unsigned i = 0; i < node->getChildCount(); i++) {
      if (node->getChild(i)->getType() != AstNodeType::Package) {
        new_node->putChild(node->getChild(i));
      }
    }
  }

  enhanceInternal(new_node, new_target, scope);
}*/

EnhancedAstNode *AstEnhancer::enhance(AstNode *node) {
  auto root = new EnhancedAstBlockNode(); // TODO is this okay?
  root->setNodeType(EnhancedAstNodeType::Program);
  root->setData(node->getData());

  Scope* scope = new Scope();
  root->setScope(scope);

  enhanceInternal(node, root, scope);

  return root;
}

