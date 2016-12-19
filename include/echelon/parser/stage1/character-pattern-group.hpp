#ifndef CHARACTER_PATTERN_GROUP_HPP_INCLUDED
#define CHARACTER_PATTERN_GROUP_HPP_INCLUDED

#include <list>

#include <echelon/parser/stage1/character-pattern-group-type-enum.hpp>
#include <echelon/parser/stage1/character-pattern-element.hpp>

class CharacterPatternGroup {
    std::list<CharacterPatternElement*> elements;

    int repeatLowerBound = 1;
    int repeatUpperBound = 1;

    bool doNotConsumeConsume = false;

    CharacterPatternGroupType type;

public:
    void addElement(CharacterPatternElement* cpe);
    std::list<CharacterPatternElement*>* getElements();

    void setRepeatLowerBound(int repeatLowerBound);
    int getRepeatLowerBound();

    void setRepeatUpperBound(int repeatUpperBound);
    int getRepeatUpperBound();

    void setDoNotConsumeConsume(bool doNotConsumeConsume);
    bool isDoNotConsumeConsume();

    void setType(CharacterPatternGroupType type);
    CharacterPatternGroupType getType();
};

#endif
