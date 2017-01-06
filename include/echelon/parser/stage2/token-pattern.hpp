#ifndef TOKEN_PATTERN_HPP_INCLUDED
#define TOKEN_PATTERN_HPP_INCLUDED

#include <vector>
#include <string>

#include <echelon/parser/stage2/token-pattern-group.hpp>

// Forward declare so that TokenPatternElement can contain TokenPattern.
class TokenPatternGroup;

class TokenPattern {
  std::vector<TokenPatternGroup *> tokenPatternGroups;

  std::string id;
public:
  void addGroup(TokenPatternGroup *tokenPatternGroup);

  std::vector<TokenPatternGroup *> *getGroups();

  void setId(std::string id);

  std::string getId();
};

#endif
