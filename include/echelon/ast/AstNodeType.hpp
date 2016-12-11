#ifndef AST_NODE_TYPE_HPP_INCLUDED
#define AST_NODE_TYPE_HPP_INCLUDED

enum class AstNodeType {
  Program,
  Block,
  Variable,
  ExplicitType,
  Package,
  Module,
  BinaryOperator,
  FunctionCall,
  Type,
  String,
  Integer,
  EqualityOperator,
  If,
  Else,
  Branches,

  #ifdef ECHELON_DEBUG
  First = Program,
  Last = Branches,
  #endif
};

#endif
