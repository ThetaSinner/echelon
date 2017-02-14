#include <echelon/linker/linker.hpp>

void Linker::linkPartial(EnhancedAstNode* target, EnhancedAstNode* partial) {
  if (target->hasChild(EnhancedAstNodeType::Block)) {
    auto block = (EnhancedAstBlockNode*) target->getChild(EnhancedAstNodeType::Block);
    auto scope = block->getScope();

    // scope pusher.
  }
}