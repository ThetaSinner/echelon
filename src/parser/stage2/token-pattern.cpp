#include <echelon/parser/stage2/token-pattern.hpp>

void TokenPattern::addGroup(TokenPatternGroup* tokenPatternGroup) {
  tokenPatternGroups.push_back(tokenPatternGroup);
}

std::vector<TokenPatternGroup*>* TokenPattern::getGroups() {
  return &tokenPatternGroups;
}

void TokenPattern::setId(std::string id) {
  this -> id = id;
}

std::string TokenPattern::getId() {
  return id;
}
