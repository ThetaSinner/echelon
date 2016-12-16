#include <echelon/parser/stage1/character-matcher-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <stdexcept>
#endif

CharacterMatcherLookup *CharacterMatcherLookup::instance = nullptr;

CharacterMatcherLookup *CharacterMatcherLookup::getInstance() {
  if (instance == nullptr) {
    instance = new CharacterMatcherLookup();
  }

  return instance;
}

void CharacterMatcherLookup::addCharacterMatcher(std::string key, CharacterMatcher matcher) {
  matcherHash.insert({key, matcher});
}

CharacterMatcher CharacterMatcherLookup::getMatcher(std::string key) {
  #ifdef ECHELON_DEBUG
  if (matcherHash.find(key) == matcherHash.end()) {
    std::cout << "Missing character matcher [" << key << "]" << std::endl;
    throw std::runtime_error("Missing character matcher");
  }
  #endif

  return matcherHash.at(key);
}