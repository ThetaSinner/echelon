#ifndef CHARACTER_PATTERN_ELEMENT_HPP_INCLUDED
#define CHARACTER_PATTERN_ELEMENT_HPP_INCLUDED

#include <string>

#include <echelon/parser/stage1/character-matcher-lookup.hpp>

class CharacterPatternElement {
    std::string data;
    CharacterMatcher matcher;

    bool useLookahead = false;
public:
    CharacterPatternElement(std::string data);

    std::string getData();
    CharacterMatcher getMatcher();
    bool isUseLookahead();
};

#endif
