#include <echelon/util/tizer.hpp>

#include <string>
#include <list>

enum class TokenType {
  SingleLineComment,
  MultiLineComment,
  Identifier,
  String
};

class Token {
private:
  std::string data;
  TokenType tokenType;
public:
  void setData(std::string data);
  std::string getData();

  void setTokenType(TokenType tokenType);
  TokenType getTokenType();
};

void Token::setData(std::string data) {
  this->data = data;
}

std::string Token::getData() {
  return data;
}

void Token::setTokenType(TokenType tokenType) {
  this->tokenType = tokenType;
}

TokenType Token::getTokenType() {
  return tokenType;
}

class Tizer {
private:
  //std::map<char
public:
  void trigger(char c) {

  }
};

std::list<std::string> tize(std::string input) {
  long input_length = input.size();
  long marker = 0;

  Tizer t;
  while (marker < input_length) {
    t.trigger(input.at(marker));
  }
}
