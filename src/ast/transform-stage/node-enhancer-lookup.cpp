#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>

NodeEnhancerLookup* NodeEnhancerLookup::instance = nullptr;

NodeEnhancerLookup* NodeEnhancerLookup::getInstance() {
  if (instance == nullptr) {
    instance = new NodeEnhancerLookup();
  }

  return instance;
}

void NodeEnhancerLookup::addNodeEnhancer(AstNodeType type, NodeEnhancer nodeEnhancer) {
  nodeEnhancers.insert({type, nodeEnhancer});
}

NodeEnhancer& NodeEnhancerLookup::getNodeEnhancer(AstNodeType type) {
  #ifdef ECHELON_DEBUG
  if (nodeEnhancers.find(type) == nodeEnhancers.end()) {
    std::cout << "Missing node enhancer for [" << EchelonLookup::getInstance() -> toString(type) << "]" << std::endl;
    throw std::runtime_error("Missing node enhancer");
  }
  #endif

  return nodeEnhancers.at(type);
}