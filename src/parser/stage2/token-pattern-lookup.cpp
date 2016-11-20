#include <echelon/parser/stage2/token-pattern-lookup.hpp>

#include <echelon/parser/stage2/pattern-translator.hpp>

TokenPatternLookup* TokenPatternLookup::self = nullptr;

TokenPatternLookup* TokenPatternLookup::getInstance() {
  if (self == nullptr) {
    self = new TokenPatternLookup();
  }

  return self;
}

void TokenPatternLookup::addTokenPattern(std::string id, std::string tokenPattern) {
  static PatternTranslator patternTranslator;
  auto pattern = patternTranslator.translate(tokenPattern);
  pattern -> setId(id);
  tokenPatterns.push_back(pattern);
}

std::list<TokenPattern*>* TokenPatternLookup::getTokenPatterns() {
  return &tokenPatterns;
}
