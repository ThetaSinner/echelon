#include <echelon/parser/parser-data/parser-stage-2-helper.hpp>

void ParserStage2Helper::mapSubProcessAsBlock(AstTransformData* astTransformData, AstNode* target) {
  auto subProcess = astTransformData->getSubProcessAstNodes();
  if (!subProcess->empty()) {
    auto block = new AstNode();
    block->setType(AstNodeType::Block);
    AstNode*& subProcessFront = subProcess->front();
    for (unsigned i = 0; i < subProcessFront->getChildCount(); i++) {
      block->putChild(subProcessFront->getChild(i));
    }
    target->putChild(block);
  }
}