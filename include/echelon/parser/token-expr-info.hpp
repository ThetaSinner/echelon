#ifndef TOKEN_EXPR_INFO_HPP
#define TOKEN_EXPR_INFO_HPP

#include <string>

#include <echelon/parser/token-type-enum.hpp>

class TokenExprInfo {
private:
  std::string friendlyName;
  TokenTypeEnum tokenType;

public:
  void setFriendlyName(std::string fname);
  std::string getFriendlyName();

  void setTokenType(TokenTypeEnum tokenTypeEnum);
  TokenTypeEnum getTokenType();
};

#endif
