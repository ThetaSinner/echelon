#ifndef NODE_ENHANCER_LOOKUP_HPP_INCLUDED
#define NODE_ENHANCER_LOOKUP_HPP_INCLUDED

#include <functional>

#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <echelon/util/echelon-lookup.hpp>
#endif

typedef std::function<EnhancedAstNode*(AstNode*, Scope)> NodeEnhancer;

class NodeEnhancerLookup {
  static NodeEnhancerLookup* instance;

  std::map<AstNodeType, NodeEnhancer> nodeEnhancers;

  NodeEnhancerLookup() {}
  NodeEnhancerLookup(const NodeEnhancerLookup&) {}
  NodeEnhancerLookup& operator=(const NodeEnhancerLookup&) {}
public:
  static NodeEnhancerLookup* getInstance();

  void addNodeEnhancer(AstNodeType type, NodeEnhancer nodeEnhancer);
  NodeEnhancer& getNodeEnhancer(AstNodeType type);
};

#endif
