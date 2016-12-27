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

bool NestedPatternLookup::isNest(std::string nest) {
  // TODO why is this returning true when it shouldn't?
  return nestedPatternLookup.find(nest) != nestedPatternLookup.end();
}

std::list<TokenPattern*>* NestedPatternLookup::getNested(std::string nest) {
  return nestedPatternLookup.at(nest);
}