#ifndef TOKEN_ENUM_HPP_INCLUDED
#define TOKEN_ENUM_HPP_INCLUDED

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
  MultiLineComment
};

#endif
