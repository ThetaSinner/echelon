#include <echelon/parser/stage2/nested-pattern-lookup.hpp>

NestedPatternLookup* NestedPatternLookup::self = nullptr;

void NestedPatternLookup::registerNest(std::string str) {
  nestedPatternLookup.insert({str, new std::list<TokenPattern*> ()});
}

NestedPatternLookup* NestedPatternLookup::getInstance() {
  if (self == nullptr) {
    self = new NestedPatternLookup();
  }

  return self;
}

void NestedPatternLookup::registerNested(std::string nest, std::string id, std::string pattern) {
  static PatternTranslator patternTranslator;

  if (nestedPatternLookup.find(nest) == nestedPatternLookup.end()) {
    registerNest(nest);
  }

  auto p = patternTranslator.translate(pattern);
  p -> setId(id);
  nestedPatternLookup.at(nest) -> push_back(p);
}

void NestedPatternLookup::forwardDeclareNested(std::string nest) {
  if (nestedPatternLookup.find(nest) == nestedPatternLookup.end()) {
    registerNest(nest);
  }
}

bool NestedPatternLookup::isNest(std::string nest) {
  return nestedPatternLookup.find(nest) != nestedPatternLookup.end();
}

std::list<TokenPattern*>* NestedPatternLookup::getNested(std::string nest) {
  return nestedPatternLookup.at(nest);
}