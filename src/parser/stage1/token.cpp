#include <echelon/parser/stage1/token.hpp>

Token::Token(std::string data, TokenType type) {
  this->data = data;
  this->type = type;
}

std::string Token::getData() const {
  return data;
}

TokenType Token::getTokenType() const {
  return type;
}
