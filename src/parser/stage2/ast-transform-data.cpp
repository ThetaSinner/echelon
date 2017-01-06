#include <echelon/parser/stage2/ast-transform-data.hpp>

void AstTransformData::setTokens(std::list<EnhancedToken *> tokens) {
  this->tokens = tokens;
}

std::list<EnhancedToken *> *AstTransformData::getTokens() {
  return &tokens;
}

void AstTransformData::setPatternMatchInfo(PatternMatchInfo *patternMatchInfo) {
  this->patternMatchInfo = patternMatchInfo;
}

PatternMatchInfo *AstTransformData::getPatternMatchInfo() {
  return patternMatchInfo;
}

void AstTransformData::setSubProcessAstNodes(std::queue<AstNode *> *subProcessAstNodes) {
  this->subProcessAstNodes = subProcessAstNodes;
}

std::queue<AstNode *> *AstTransformData::getSubProcessAstNodes() {
  return subProcessAstNodes;
}

void AstTransformData::setNestedAstNodes(std::queue<AstNode *> *nestedAstNodes) {
  this->nestedAstNodes = nestedAstNodes;
}

std::queue<AstNode *> *AstTransformData::getNestedAstNodes() {
  return nestedAstNodes;
}
