#ifndef TOKEN_ENUM_HPP_INCLUDED
#define TOKEN_ENUM_HPP_INCLUDED

#include <vector>
#include <string>

enum class TokenType {
  Identifier,
  Decimal,
  Integer,
  String,
  SingleLineComment,
  MultiLineComment,
  LogicalConjunction,
  LogicalDisjunction,
  LogicalNegation,
  LogicalEquality,
  BlockDelimiterOpen,
  BlockDelimiterClose,
  ParenthesisOpen,
  ParenthesisClose,
  Assignment,
  AddOperator,
  SubtractOperator,
  MultiplyOperator,
  DivideOperator,
  Comma,
  StructureOperator,
  ForwardArrowOperator,
  EllipsisOperator,
  AccessOperator,

#ifdef ECHELON_DEBUG
  First = Identifier,
  Last = AccessOperator,
#endif
};

#endif
