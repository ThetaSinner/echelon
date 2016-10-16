#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include <string>
#include <echelon/parser/token-type-enum.hpp>

class Token {
private:
  std::string data;
  TokenTypeEnum type;

public:
  Token() {}
  Token(std::string data, TokenTypeEnum type);

  std::string getData();
  TokenTypeEnum getTokenType();
};

#endif
