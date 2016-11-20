#include <echelon/parser/stage2/matcher-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#endif

MatcherLookup *MatcherLookup::self = nullptr;

MatcherLookup* MatcherLookup::getInstance() {
  if (self == nullptr) {
    self = new MatcherLookup();
  }

  return self;
}

void MatcherLookup::addMatcher(std::string key, Matcher* matcher) {
  matcherHash.insert({key, matcher});
}

Matcher* MatcherLookup::getMatcher(std::string key) {
  #ifdef ECHELON_DEBUG
  if (matcherHash.find(key) == matcherHash.end()) {
    std::cout << "Missing matcher for " << key << std::endl;
    throw std::runtime_error("Missing matcher");
  }
  #endif

  return matcherHash.at(key);
}
