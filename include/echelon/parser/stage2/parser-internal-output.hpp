#ifndef PARSER_INTERNAL_OUTPUT_HPP_INCLUDED
#define PARSER_INTERNAL_OUTPUT_HPP_INCLUDED

#include <echelon/ast/AstNode.hpp>

class ParserInternalOutput {
  AstNode *astNode;
  int tokensConsumedCount = 0;
public:
  void setAstNode(AstNode* astNode);
  AstNode* getAstNode();

  void setTokensConsumedCount(int tokensConsumedCount);
  int getTokensConsumedCount();
};

#endif
