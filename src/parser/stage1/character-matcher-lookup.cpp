#include <echelon/parser/stage1/character-matcher-lookup.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>

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
    LoggerSharedInstance::get()->at(Level::Fatal) << "Missing character matcher [" << key << "]\n";
    throw std::runtime_error("Missing character matcher");
  }
#endif

  return matcherHash.at(key);
}