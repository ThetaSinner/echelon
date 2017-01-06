#include <echelon/parser/stage2/parser-internal-input.hpp>

void ParserInternalInput::setTokens(std::list<Token *> *tokens) {
  this->tokens = tokens;
}

std::list<Token *> *ParserInternalInput::getTokens() {
  return tokens;
}

void ParserInternalInput::setSubProcessFinishGroup(TokenPatternGroup *subProcessFinishGroup) {
  this->subProcessFinishGroup = subProcessFinishGroup;
}

TokenPatternGroup *ParserInternalInput::getSubProcessFinishGroup() {
  return subProcessFinishGroup;
}

void ParserInternalInput::setNestedPatterns(std::list<TokenPattern *> *nestedPatterns) {
  this->nestedPatterns = nestedPatterns;
}

std::list<TokenPattern *> *ParserInternalInput::getNestedPatterns() const {
  return nestedPatterns;
}

void ParserInternalInput::setUseNestedPatterns(bool useNestedPatterns) {
  this->useNestedPatterns = useNestedPatterns;
}

bool ParserInternalInput::isUseNestedPatterns() {
  return useNestedPatterns;
}
