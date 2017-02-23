#ifndef ENHANCED_AST_NODE_TYPE_ENUM_HPP_INCLUDED
#define ENHANCED_AST_NODE_TYPE_ENUM_HPP_INCLUDED

enum class EnhancedAstNodeType {
  None,

  Package,
  Module,

  CustomType,

  Block,

  Program,
  Variable,
  PrimitiveValue,
  TypeName,

  BinaryOperator,

  Function,
  FunctionParamDefinition,
  FunctionParamDefinitions,

  VariableValue,
  ExpressionGroup,

  NameStructure,

  SingleLineComment,

  Expression,

  AccessExpression,
  AccessStructure,

  FunctionCall,
  FunctionCallParams,

  // Not for ast structure use.
  First = None,
  Last = FunctionCallParams,
};

#endif