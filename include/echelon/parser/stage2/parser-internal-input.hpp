#ifndef PARSER_INTERNAL_INPUT_HPP_INCLUDED
#define PARSER_INTERNAL_INPUT_HPP_INCLUDED

#include <list>

#include <echelon/parser/token.hpp>
#include <echelon/parser/stage2/token-pattern-group.hpp>

class ParserInternalInput {
  std::list<Token*>* tokens;

  TokenPatternGroup *subProcessFinishGroup = nullptr;

public:
  void setTokens(std::list<Token*>* tokens);
  std::list<Token*>* getTokens();

  void setSubProcessFinishGroup(TokenPatternGroup* subProcessFinishGroup);
  TokenPatternGroup* getSubProcessFinishGroup();
};

#endif
