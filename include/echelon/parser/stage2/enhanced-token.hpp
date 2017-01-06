#ifndef ENHANCED_TOKEN_HPP_INCLUDED
#define ENHANCED_TOKEN_HPP_INCLUDED

#include <string>
#include <echelon/parser/stage1/token.hpp>
#include <echelon/parser/token-type-enum.hpp>

class EnhancedToken {
private:
  TokenType tokenType;
  std::string data;

  bool keyword = false;
  bool dataTypeKeyword;
public:
  EnhancedToken(Token *t);

  TokenType getTokenType();

  std::string getData();

  bool isKeyword();

  bool isDataTypeKeyword();
};

#endif
