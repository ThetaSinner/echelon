#include <echelon/parser/token.hpp>

Token::Token(std::string data, TokenTypeEnum type) {
  this->data = data;
  this->type = type;
}

std::string Token::getData() const {
  return data;
}

TokenTypeEnum Token::getTokenType() const {
  return type;
}
