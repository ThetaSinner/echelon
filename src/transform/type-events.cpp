#include <echelon/transform/type-events.hpp>

void TypeEvents::registerRefersToMissingFunction(EnhancedAstNode* dependsOnMissing, TypeEventData* typeEventData, EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target, TransformWorkingData* transformWorkingData) {
  #ifdef ECHELON_DEBUG
  if (dependsOnMissing->getNodeType() != EnhancedAstNodeType::FunctionCall) {
    throw std::runtime_error("TypeEvents: Error node should be a function call.");
  }
  #endif

  auto eventContainer = &transformWorkingData->getEventContainer();

  eventContainer->addEventListener("function-name-added:" + dependsOnMissing->getData(), [=](void* data) -> EventListenerResult {
    EventListenerResult eventListenerResult;

    auto added = (EnhancedAstNode*) data;
    if (added->getNodeType() != EnhancedAstNodeType::Function) {
      return eventListenerResult;
    }

    typeEventData->setEventGroupSize(typeEventData->getEventGroupSize() - 1);
    if (typeEventData->getEventGroupSize() == 0) {
      TypeDeducer::deduceTypes(expressionNode, scope, target, transformWorkingData);
    }

    eventListenerResult.setRemoveListener(true);
    return eventListenerResult;
  });
}

void TypeEvents::triggerFunctionAdded(EnhancedAstNode* function, TransformWorkingData* transformWorkingData) {
  transformWorkingData->getEventContainer().triggerEvent("function-name-added:" + function->getData(), function);
}