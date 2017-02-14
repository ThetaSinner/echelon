#ifndef NODE_ENHANCER_LOOKUP_HPP_INCLUDED
#define NODE_ENHANCER_LOOKUP_HPP_INCLUDED

#include <functional>

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>
#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>

#ifdef ECHELON_DEBUG

#include <iostream>
#include <echelon/util/echelon-lookup.hpp>

#endif

typedef std::function<AstNodeEnhancerOutputData(AstNodeEnhancerInputData&)> NodeEnhancer;

class NodeEnhancerLookup {
  static NodeEnhancerLookup *instance;

  std::map<AstNodeType, NodeEnhancer> nodeEnhancers;

  NodeEnhancerLookup() {}

  NodeEnhancerLookup(const NodeEnhancerLookup &) {}

  NodeEnhancerLookup &operator=(const NodeEnhancerLookup &) {}

public:
  static NodeEnhancerLookup *getInstance();

  void addNodeEnhancer(AstNodeType type, NodeEnhancer nodeEnhancer);

  NodeEnhancer &getNodeEnhancer(AstNodeType type);
};

#endif
