#include <echelon/parser/stage2/parser-internal-output.hpp>

void ParserInternalOutput::setAstNode(AstNode* astNode) {
  this -> astNode = astNode;
}

AstNode* ParserInternalOutput::getAstNode() {
  return astNode;
}

void ParserInternalOutput::setTokensConsumedCount(int tokensConsumedCount) {
  this -> tokensConsumedCount = tokensConsumedCount;
}
int ParserInternalOutput::getTokensConsumedCount() {
  return tokensConsumedCount;
}
