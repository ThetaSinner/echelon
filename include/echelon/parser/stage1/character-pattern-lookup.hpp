#ifndef CHARACTER_PATTERN_LOOKUP_HPP_INCLUDED
#define CHARACTER_PATTERN_LOOKUP_HPP_INCLUDED

#include <list>

#include <echelon/parser/stage1/character-pattern.hpp>

class CharacterPatternLookup {
  static CharacterPatternLookup *instance;

  std::list<CharacterPattern *> characterPatternList;

  CharacterPatternLookup() {}

  CharacterPatternLookup(const CharacterPatternLookup &) {}

  CharacterPatternLookup &operator=(const CharacterPatternLookup &) {}

public:
  static CharacterPatternLookup *getInstance();

  void addCharacterPattern(CharacterPattern *characterPattern);

  std::list<CharacterPattern *> *getCharacterPatternList();
};

#endif
