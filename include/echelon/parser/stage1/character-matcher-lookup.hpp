#ifndef CHARACTER_MATCHER_LOOKUP_HPP_INCLUDED
#define CHARACTER_MATCHER_LOOKUP_HPP_INCLUDED

#include <map>
#include <string>
#include <functional>

typedef std::function<bool(char)> CharacterMatcher;

class CharacterMatcherLookup {
    static CharacterMatcherLookup *instance;

    std::map<std::string, CharacterMatcher> matcherHash;

    CharacterMatcherLookup() {}
    CharacterMatcherLookup(const CharacterMatcherLookup&) {}
    CharacterMatcherLookup& operator=(const CharacterMatcherLookup&) {}
public:
    static CharacterMatcherLookup* getInstance();

    void addCharacterMatcher(std::string key, CharacterMatcher matcher);
    CharacterMatcher getMatcher(std::string key);
};


#endif