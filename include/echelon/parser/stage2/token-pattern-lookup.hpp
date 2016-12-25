#ifndef TOKEN_PATTERN_LOOKUP_HPP_INCLUDED
#define TOKEN_PATTERN_LOOKUP_HPP_INCLUDED

#include <string>
#include <list>

#include <echelon/parser/stage2/token-pattern.hpp>

class TokenPatternLookup {
  static TokenPatternLookup *self;

  std::list<TokenPattern*> tokenPatterns;

  TokenPatternLookup() {}
  TokenPatternLookup(const TokenPatternLookup& tpl) {}
  TokenPatternLookup& operator=(const TokenPatternLookup& tpl) {}
public:
  static TokenPatternLookup* getInstance();

  void addTokenPattern(std::string id, std::string tokenPattern);
  std::list<TokenPattern*>* getTokenPatterns();


};

#endif
