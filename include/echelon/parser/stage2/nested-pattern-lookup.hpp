#ifndef NESTED_PATTERN_LOOKUP_HPP_INCLUDED
#define NESTED_PATTERN_LOOKUP_HPP_INCLUDED

#include <string>
#include <map>
#include <list>

class NestedPatternLookup {
  static NestedPatternLookup *self;

  std::map<std::string, std::list<std::string>*> nestedPatternLookup;

  void registerNest(std::string str) {
    nestedPatternLookup.insert({str, new std::list<std::string> ()});
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
    if (nestedPatternLookup.find(nest) == nestedPatternLookup.end()) {
      registerNest(nest);
    }

    nestedPatternLookup.at(nest) -> push_back(pattern);
  }

  bool isNest(std::string nest) {
    return nestedPatternLookup.find(nest) != nestedPatternLookup.end();
  }

  std::list<std::string>* getNested(std::string nest) {
    return nestedPatternLookup.at(nest);
  }
};

#endif
