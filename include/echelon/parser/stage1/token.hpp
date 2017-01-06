#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include <string>
#include <echelon/parser/token-type-enum.hpp>

class Token {
private:
  std::string data;
  TokenType type;

public:
  Token() {}

  Token(std::string data, TokenType type);

  std::string getData() const;

  TokenType getTokenType() const;
};

#endif
