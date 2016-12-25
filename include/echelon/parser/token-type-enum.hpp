#ifndef TOKEN_ENUM_HPP_INCLUDED
#define TOKEN_ENUM_HPP_INCLUDED

#include <vector>
#include <string>

enum class TokenType {
  String,
  Identifier,
  Integer,
  Float,
  BlockDelimO,
  BlockDelimC,
  Paren,
  ParenO,
  ParenC,
  OperatorAdd,
  OperatorSubtract,
  OperatorMultiply,
  OperatorDivide,
  SqBracket,
  Assign,
  Equality,
  SingleLineComment,
  MultiLineComment,
  StructureOperator,
  CommaOperator,
  AndOperator,
  OrOperator,

  #ifdef ECHELON_DEBUG
  First = String,
  Last = OrOperator,
  #endif
};

#endif
