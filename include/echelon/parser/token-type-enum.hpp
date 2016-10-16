#ifndef TOKEN_ENUM_HPP_INCLUDED
#define TOKEN_ENUM_HPP_INCLUDED

enum class TokenTypeEnum {
  String,
  Identifier,
  Integer,
  BlockDelim,
  Paren,
  SqBracket,
  Assign,
  Equality,
  LineComment,
  MultiLineComment
};

#endif
