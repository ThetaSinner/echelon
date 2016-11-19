#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <vector>
#include <list>
#include <queue>

#include <echelon/parser/stage2/ast-construction-manager.hpp>
#include <echelon/parser/stage2/token-pattern.hpp>
#include <echelon/parser/stage2/pattern-match-info.hpp>
#include <echelon/parser/stage2/parser-internal-input.hpp>
#include <echelon/parser/stage2/parser-internal-output.hpp>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/parser/stage2/ast-transform-data.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>

class Parser2 {
private:
  std::vector<TokenPattern*> tokenPatterns;

  ParserInternalOutput _parse(ParserInternalInput& parserInternalInput);

  bool simpleGroupMatch(std::list<Token*>& tokens, TokenPatternGroup* group);

  ParserInternalOutput subProcess(std::list<Token*>::iterator start, std::list<Token*>::iterator end, TokenPatternGroup* nextGroup);
public:
  AstNode* parse(std::list<Token*> tokens);

  void addTokenPattern(TokenPattern* tokenPattern);

  void addTokenPattern(std::string id, std::string tokenPattern);
};

#endif