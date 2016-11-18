#include <echelon/parser/stage2/token-pattern-group.hpp>

void TokenPatternGroup::addElement(TokenPatternElement* tokenPatternElement) {
  tokenPatternElements.push_back(tokenPatternElement);
}

std::vector<TokenPatternElement*>* TokenPatternGroup::getElements() {
  return &tokenPatternElements;
}

void TokenPatternGroup::setRepeatLowerBound(int repeatLowerBound) {
  this -> repeatLowerBound = repeatLowerBound;
}

int TokenPatternGroup::getRepeatLowerBound() const {
  return repeatLowerBound;
}

void TokenPatternGroup::setRepeatUpperBound(int repeatUpperBound) {
  this -> repeatUpperBound = repeatUpperBound;
}

int TokenPatternGroup::getRepeatUpperBound() const {
  return repeatUpperBound;
}
