#ifndef MATCHER_LOOKUP_HPP_INCLUDED
#define MATCHER_LOOKUP_HPP_INCLUDED

#include <string>
#include <map>
#include <echelon/parser/stage2/matcher.hpp>

class MatcherLookup {
  static MatcherLookup *self;

  std::map<std::string, Matcher*> matcherHash;

  MatcherLookup() {};
  MatcherLookup(const MatcherLookup& _) {}
  void operator=(const MatcherLookup& _) {}
public:
  static MatcherLookup* getInstance();

  void addMatcher(std::string key, Matcher* matcher);
  Matcher* getMatcher(std::string key);
};



#endif
