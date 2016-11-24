#ifndef AST_NODE_TYPE_HPP_INCLUDED
#define AST_NODE_TYPE_HPP_INCLUDED

enum class AstNodeType {
  Program,
  Block,
  AssignmentExpr,
  ExplicitType,
  Package,
  Module,
  BinaryOperator,
  FunctionCall,
};

#endif
