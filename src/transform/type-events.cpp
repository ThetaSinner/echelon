#include <echelon/transform/type-events.hpp>

void TypeEvents::registerRefersToMissingFunction(EnhancedAstNode* dependsOnMissing, EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target, TransformWorkingData* transformWorkingData) {
  #ifdef ECHELON_DEBUG
  if (dependsOnMissing->getNodeType() != EnhancedAstNodeType::FunctionCall) {
    throw std::runtime_error("TypeEvents: Error node should be a function call.");
  }
  #endif

  auto eventContainer = &transformWorkingData->getEventContainer();

  eventContainer->addEventListener("function-name-added:" + dependsOnMissing->getData(), [=](EventKey& eventKey, void* data) -> void {
    auto added = (EnhancedAstNode*) data;
    if (added->getNodeType() != EnhancedAstNodeType::Function) {
      return;
    }

    TypeDeducer::deduceTypes(expressionNode, scope, target, transformWorkingData);
    eventContainer->removeEventListener(eventKey);
  });
}

void TypeEvents::triggerFunctionAdded(EnhancedAstNode* function, TransformWorkingData* transformWorkingData) {
  transformWorkingData->getEventContainer().triggerEvent("function-name-added:" + function->getData(), function);
}