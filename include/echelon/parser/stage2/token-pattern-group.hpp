#ifndef TOKEN_PATTERN_GROUP_HPP_INCLUDED
#define TOKEN_PATTERN_GROUP_HPP_INCLUDED

#include <vector>

#include <echelon/parser/stage2/token-pattern-element.hpp>

class TokenPatternGroup {
  std::vector<TokenPatternElement*> tokenPatternElements;

  int repeatLowerBound = 1;
  int repeatUpperBound = 1;
public:
  void addElement(TokenPatternElement* tokenPatternElement);
  std::vector<TokenPatternElement*>* getElements();

  void setRepeatLowerBound(int repeatLowerBound);
  int getRepeatLowerBound() const;

  void setRepeatUpperBound(int repeatUpperBound);
  int getRepeatUpperBound() const;
};

#endif
