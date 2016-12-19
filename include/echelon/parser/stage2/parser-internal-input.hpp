#ifndef PARSER_INTERNAL_INPUT_HPP_INCLUDED
#define PARSER_INTERNAL_INPUT_HPP_INCLUDED

#include <list>

#include <echelon/parser/stage1/token.hpp>
#include <echelon/parser/stage2/token-pattern-group.hpp>

class ParserInternalInput {
  std::list<Token*>* tokens;

  TokenPatternGroup *subProcessFinishGroup = nullptr;

  std::list<TokenPattern*>* nestedPatterns = nullptr;
  bool useNestedPatterns = false;

public:
  void setTokens(std::list<Token*>* tokens);
  std::list<Token*>* getTokens();

  void setSubProcessFinishGroup(TokenPatternGroup* subProcessFinishGroup);
  TokenPatternGroup* getSubProcessFinishGroup();

  void setNestedPatterns(std::list<TokenPattern*>* nestedPatterns);
  std::list<TokenPattern*>* getNestedPatterns() const;

  void setUseNestedPatterns(bool useNestedPatterns);
  bool isUseNestedPatterns();
};

#endif
