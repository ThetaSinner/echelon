#ifndef TYPE_EVENTS_HPP_INCLUDED
#define TYPE_EVENTS_HPP_INCLUDED

#include <echelon/util/event/event-container.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node-type-enum.hpp>
// TODO remove circular dependency by extracting common functionality.
#include <echelon/transform/type-deducer.hpp>

class TypeEvents {
public:
  static void registerRefersToMissingFunction(EnhancedAstNode* dependsOnMissing, EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target, TransformWorkingData* transformWorkingData);
  static void triggerFunctionAdded(EnhancedAstNode* function, TransformWorkingData* transformWorkingData);
};

#endif

