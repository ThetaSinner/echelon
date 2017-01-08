#include <echelon/parser/stage2/matcher-lookup.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>

#ifdef ECHELON_DEBUG

#include <iostream>

#endif

MatcherLookup *MatcherLookup::self = nullptr;

MatcherLookup *MatcherLookup::getInstance() {
  if (self == nullptr) {
    self = new MatcherLookup();
  }

  return self;
}

void MatcherLookup::addMatcher(std::string key, Matcher *matcher) {
  matcherHash.insert({key, matcher});
}

Matcher *MatcherLookup::getMatcher(std::string key) {
#ifdef ECHELON_DEBUG
  if (matcherHash.find(key) == matcherHash.end()) {
    LoggerSharedInstance::get()->at(Level::Fatal) << "Missing matcher for [" << key << "]\n";
    throw std::runtime_error("Missing matcher");
  }
#endif

  return matcherHash.at(key);
}

std::map<std::string, Matcher*>* MatcherLookup::getMatchers() {
  return &matcherHash;
};
