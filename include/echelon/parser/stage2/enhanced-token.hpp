#ifndef ENHANCED_TOKEN_HPP_INCLUDED
#define ENHANCED_TOKEN_HPP_INCLUDED

#include <string>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-type-enum.hpp>

class EnhancedToken {
private:
  TokenTypeEnum tokenType;
  std::string data;

  bool keyword = false;
  bool dataTypeKeyword;
public:
  EnhancedToken(Token* t);

  TokenTypeEnum getTokenType();
  std::string getData();

  bool isKeyword();
  bool isDataTypeKeyword();
};

#endif
