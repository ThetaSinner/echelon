#ifndef AST_NODE_TYPE_ENUM_HPP_INCLUDED
#define AST_NODE_TYPE_ENUM_HPP_INCLUDED

enum class AstNodeType {
  Program,
  Block,
  Variable,
  ExplicitType,
  Package,
  Enum,
  EnumConstant,
  EnumConstants,
  Module,
  BinaryOperator,
  BooleanBinaryOperator,
  Function,
  FunctionParamDefinitions,
  FunctionParamDefinition,
  FunctionCall,
  FunctionCallParams,
  FunctionCallParam,
  Type,
  String,
  Integer,
  Float,
  Boolean,
  EqualityOperator,
  If,
  ElseIf,
  Else,
  Branches,
  SingleLineComment,
  MultiLineComment,
  Until,
  While,
  Each,
  Condition,
  Behaviour,
  ExpressionRange,
  ExpressionRangeBegin,
  ExpressionRangeEnd,
  Iterator,
  ExprGroup,
  BooleanInvert,

#ifdef ECHELON_DEBUG
  First = Program,
  Last = BooleanInvert,
#endif
};

#endif
