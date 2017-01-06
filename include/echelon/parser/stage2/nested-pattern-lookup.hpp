#ifndef NESTED_PATTERN_LOOKUP_HPP_INCLUDED
#define NESTED_PATTERN_LOOKUP_HPP_INCLUDED

#include <string>
#include <map>
#include <list>

#include <echelon/parser/stage2/token-pattern.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>

class NestedPatternLookup {
  static NestedPatternLookup *self;

  std::map<std::string, std::list<TokenPattern *> *> nestedPatternLookup;

  void registerNest(std::string str);

  NestedPatternLookup() {}

  NestedPatternLookup(const NestedPatternLookup &npl) {}

  NestedPatternLookup &operator=(const NestedPatternLookup &npl) {}

public:
  static NestedPatternLookup *getInstance();

  void registerNested(std::string nest, std::string id, std::string pattern);

  void forwardDeclareNested(std::string nest);

  bool isNest(std::string nest);

  std::list<TokenPattern *> *getNested(std::string nest);
};

#endif
