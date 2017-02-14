#include <echelon/linker/linker.hpp>

void Linker::linkPartial(EnhancedAstNode* target, EnhancedAstNode* partial) {
  if (target->hasChild(EnhancedAstNodeType::Block)) {
    auto block = (EnhancedAstBlockNode*) target->getChild(EnhancedAstNodeType::Block);
    auto scope = block->getScope();

    // Add the new item. Note that this may create an invalid model, but this will cause an error on push to scope.
    block->putChild(partial->getChild(0));

    // Make the new item aware of its parent scope.
    if (partial->hasChild(EnhancedAstNodeType::Block)) {
      auto partialScope = ((EnhancedAstBlockNode*) partial->getChild(EnhancedAstNodeType::Block))->getScope();
      ScopeHelper::makeParentOf(partialScope, scope);
    }

    // Add the new item to the scope. If there is a clash then the compiler will exit.
    // Otherwise, events on the new item should be triggered somehow..
    ScopePusher::push(scope, partial);

    // if partial.hasUndetermined values
    // then partial.triggerContextChange
  }
}