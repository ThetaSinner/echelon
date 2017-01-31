#ifndef TOKEN_HPP_INCLUDED
#define TOKEN_HPP_INCLUDED

#include <string>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/stage1/source-map-data.hpp>

class Token {
private:
  std::string data;
  TokenType type;
  SourceMapData sourceMapData;

public:
  Token() {}

  Token(std::string data, TokenType type);

  std::string getData() const;

  TokenType getTokenType() const;

  void setSourceMapData(SourceMapData sourceMapData);
  SourceMapData getSourceMapData();
};

#endif
