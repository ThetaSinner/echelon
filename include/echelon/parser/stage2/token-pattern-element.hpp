#ifndef TOKEN_PATTERN_ELEMENT_HPP_INCLUDED
#define TOKEN_PATTERN_ELEMENT_HPP_INCLUDED

#include <string>
#include <list>

#include <echelon/parser/stage2/matcher.hpp>
#include <echelon/parser/stage2/token-pattern.hpp>

// Forward declare so that TokenPatternElement can contain TokenPattern.
class TokenPattern;

class TokenPatternElement {
private:
  std::string data;
  Matcher *matcher;

  std::list<TokenPattern *> *nestedPatterns;
  bool useNestedPatterns = false;

  bool subProcess = false;
public:
  TokenPatternElement(std::string element);

  std::string getData() const;

  Matcher *getMatcher() const;

  std::list<TokenPattern *> *getNestedPatterns() const;

  bool isSubProcess();

  bool isUseNestedPatterns();
};


#endif
