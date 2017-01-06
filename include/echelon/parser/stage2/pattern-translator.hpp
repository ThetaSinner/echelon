#ifndef PATTERN_TRANSLATOR_HPP_INCLUDED
#define PATTERN_TRANSLATOR_HPP_INCLUDED

#include <string>

#include <echelon/parser/stage2/token-pattern-element.hpp>
#include <echelon/parser/stage2/token-pattern.hpp>

class PatternTranslator {
private:
  TokenPatternElement *readIdentifier(std::string::iterator &i, std::string &pattern);

public:
  TokenPattern *translate(std::string pattern);
};

#endif
