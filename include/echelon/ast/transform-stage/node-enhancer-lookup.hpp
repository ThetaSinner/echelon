#ifndef NODE_ENHANCER_LOOKUP_HPP_INCLUDED
#define NODE_ENHANCER_LOOKUP_HPP_INCLUDED

#include <functional>

#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <echelon/parser/echelon-lookup.hpp>
#endif

typedef std::function<EnhancedAstNode*(AstNode*, Scope)> NodeEnhancer;

class NodeEnhancerLookup {
  static NodeEnhancerLookup* instance;

  std::map<AstNodeType, NodeEnhancer> nodeEnhancers;

  NodeEnhancerLookup() {}
  NodeEnhancerLookup(const NodeEnhancerLookup&) {}
  NodeEnhancerLookup& operator=(const NodeEnhancerLookup&) {}
public:
  static NodeEnhancerLookup* getInstance() {
    if (instance == nullptr) {
      instance = new NodeEnhancerLookup();
    }

    return instance;
  }

  void addNodeEnhancer(AstNodeType type, NodeEnhancer nodeEnhancer) {
    nodeEnhancers.insert({type, nodeEnhancer});
  }

  NodeEnhancer& getNodeEnhancer(AstNodeType type) {
    #ifdef ECHELON_DEBUG
    if (nodeEnhancers.find(type) == nodeEnhancers.end()) {
      std::cout << "Missing node enhancer for [" << EchelonLookup::getInstance() -> toString(type) << "]" << std::endl;
      throw std::runtime_error("Missing node enhancer");
    }
    #endif

    return nodeEnhancers.at(type);
  }
};

#endif
