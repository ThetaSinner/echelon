#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/echelon-lookup.hpp>
#endif

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
    LoggerSharedInstance::get()->at(Level::Fatal) << "Missing node enhancer for [" << EchelonLookup::getInstance() -> toString(type) << "]\n";
    throw std::runtime_error("Missing node enhancer");
  }
  #endif

  return nodeEnhancers.at(type);
}