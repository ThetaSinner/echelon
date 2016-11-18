#ifndef TOKEN_PATTERN_ELEMENT_HPP_INCLUDED
#define TOKEN_PATTERN_ELEMENT_HPP_INCLUDED

#include <string>

#include <echelon/parser/stage2/matcher.hpp>

class TokenPatternElement {
private:
  std::string data;
  Matcher* matcher;

  bool subProcess = false;
public:
  TokenPatternElement(std::string element);

  std::string getData() const;
  Matcher* getMatcher() const;

  bool isSubProcess();
};


#endif
