#ifndef CHARACTER_PATTERN_GROUP_HPP_INCLUDED
#define CHARACTER_PATTERN_GROUP_HPP_INCLUDED

#include <list>

#include <echelon/parser/stage1/character-pattern-group-type-enum.hpp>
#include <echelon/parser/stage1/character-pattern-element.hpp>

class CharacterPatternGroup {
    std::list<CharacterPatternElement*> elements;

    bool repeatable = false;

    CharacterPatternGroupType type;

public:
    void addElement(CharacterPatternElement* cpe);
    std::list<CharacterPatternElement*>* getElements();

    void setRepeatable(bool repeatable);
    bool isRepeatable();

    void setType(CharacterPatternGroupType type);
    CharacterPatternGroupType getType();
};

#endif
