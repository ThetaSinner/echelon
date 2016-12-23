#include <echelon/transform/ast-enhancer.hpp>

#include <map>
#include <sstream>
#include <list>
#include <functional>

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>

void mapSubNodes(AstNode* source, EnhancedAstNode* target, Scope& scope) {
  for (unsigned i = 0; i < source -> getChildCount(); i++) {
    #ifdef ECHELON_DEBUG
    if (nodeEnhancers.find(source -> getChild(i) -> getType()) == nodeEnhancers.end()) {
      std::cout << "Missing node enhancer [" << EchelonLookup::getInstance() -> toString(source -> getChild(i) -> getType()) << "]" << std::endl;
      throw std::runtime_error("Mising node enhancer");
    }
    #endif

    target -> putChild(NodeEnhancerLookup::getInstance() -> getNodeEnhancer(source -> getChild(i) -> getType())(source -> getChild(i), scope));
  }
}

void enhanceInternal(AstNode* node, EnhancedAstNode* target, Scope scope) {
  for (unsigned i = 0; i < node -> getChildCount(); i++) {
    auto enhancedNode = new EnhancedAstNode();

    if (node -> getChild(i) -> getType() == AstNodeType::Variable) {
      enhancedNode -> setNodeType(EnhancedAstNodeType::Variable);
      auto data = node -> getChild(i) -> getData();

      // This is the first time we've seen this variable in this scope, add it.
      if (!scope.hasVariable(data)) {
        enhancedNode -> setNodeSubType(EnhancedAstNodeSubType::Declaration);
        scope.addVariable(data, node -> getChild(i));
      }
      else {
        // The variable has been seen before. Check that there is no type declaration.
        if (node -> getChild(i) -> getChild(0) -> getType() == AstNodeType::Type) {
          std::stringstream ss;
          ss << "Error, redeclaration of variable [" << data << "].";
          throw std::runtime_error(ss.str());
        }

        enhancedNode -> setNodeSubType(EnhancedAstNodeSubType::Assign);
      }

      mapSubNodes(node -> getChild(i), enhancedNode, scope);
    }
    else if (node -> getChild(i) -> getType() == AstNodeType::Function) {
      // map function and store on scope.
    }
    else {
      // This is not new data being declared, so map it and ensure that all references to variables and functions are valid.
      target -> putChild(NodeEnhancerLookup::getInstance() -> getNodeEnhancer(node -> getChild(i) -> getType())(node -> getChild(i), scope));
    }

    target -> putChild(enhancedNode);
  }
}

EnhancedAstNode* AstEnhancer::enhance(AstNode* node) {
  EnhancedAstNode *root = new EnhancedAstNode();
  root -> setNodeType(EnhancedAstNodeType::Program);

  Scope scope;
  enhanceInternal(node, root, scope);

  return root;
}

