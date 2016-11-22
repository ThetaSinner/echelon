#ifndef NESTED_PATTERN_LOOKUP_HPP_INCLUDED
#define NESTED_PATTERN_LOOKUP_HPP_INCLUDED

#include <string>
#include <map>
#include <list>

#include <echelon/parser/stage2/token-pattern.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>

class NestedPatternLookup {
  static NestedPatternLookup *self;

  std::map<std::string, std::list<TokenPattern*>*> nestedPatternLookup;

  void registerNest(std::string str) {
    nestedPatternLookup.insert({str, new std::list<TokenPattern*> ()});
  }

  NestedPatternLookup() {}
  NestedPatternLookup(const NestedPatternLookup& npl) {}
  NestedPatternLookup& operator=(const NestedPatternLookup& npl) {}
public:
  static NestedPatternLookup* getInstance() {
    if (self == nullptr) {
      self = new NestedPatternLookup();
    }

    return self;
  }

  void registerNested(std::string nest, std::string pattern) {
    static PatternTranslator patternTranslator;

    if (nestedPatternLookup.find(nest) == nestedPatternLookup.end()) {
      registerNest(nest);
    }

    nestedPatternLookup.at(nest) -> push_back(patternTranslator.translate(pattern));
  }

  bool isNest(std::string nest) {
    return nestedPatternLookup.find(nest) != nestedPatternLookup.end();
  }

  std::list<TokenPattern*>* getNested(std::string nest) {
    return nestedPatternLookup.at(nest);
  }
};

#endif
