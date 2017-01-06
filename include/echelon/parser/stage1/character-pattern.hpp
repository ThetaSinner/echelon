#ifndef CHARACTER_PATTERN_HPP_INCLUDED
#define CHARACTER_PATTERN_HPP_INCLUDED

#include <list>

#include <echelon/parser/stage1/character-pattern-group.hpp>
#include <echelon/parser/token-type-enum.hpp>

class CharacterPattern {
  std::list<CharacterPatternGroup *> groups;

  TokenType type;
public:
  void addGroup(CharacterPatternGroup *cpg);

  std::list<CharacterPatternGroup *> *getGroups();

  void setTokenType(TokenType type);

  TokenType getTokenType();
};

#endif
