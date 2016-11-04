#ifndef TOKEN_ENUM_HPP_INCLUDED
#define TOKEN_ENUM_HPP_INCLUDED

#include <string>

enum class TokenTypeEnum {
  String,
  Identifier,
  Integer,
  BlockDelimO,
  BlockDelimC,
  Paren,
  SqBracket,
  Assign,
  Equality,
  SingleLineComment,
  MultiLineComment,
  StructureOperator,
};

std::string toString(TokenTypeEnum tokenType);

#endif
