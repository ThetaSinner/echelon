#ifndef CHARACTER_PATTERN_HPP_INCLUDED
#define CHARACTER_PATTERN_HPP_INCLUDED

#include <list>

#include <echelon/parser/stage1/character-pattern-group.hpp>

class CharacterPattern {
    std::list<CharacterPatternGroup*> groups;
public:
    void addGroup(CharacterPatternGroup* cpg);
    std::list<CharacterPatternGroup*>* getGroups();
};

#endif
