#ifndef echelon_TOKENIZER_INCLUDED
#define echelon_TOKENIZER_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/token-expr.hpp>

class Tokenizer
{
private:
  std::map<TokenTypeEnum, TokenExpr> tokenExprMap;
public:
  Tokenizer();

  std::vector<Token> tokenize(std::string in);

  std::map<TokenTypeEnum, TokenExpr> getTokenExprMap();
};

#endif
