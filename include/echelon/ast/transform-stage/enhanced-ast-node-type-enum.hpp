#ifndef ENHANCED_AST_NODE_TYPE_ENUM_HPP_INCLUDED
#define ENHANCED_AST_NODE_TYPE_ENUM_HPP_INCLUDED

enum class EnhancedAstNodeType {
  None,

  Program,
  Variable,
  PrimitiveValue,
  Type,

  Function,
  FunctionParamDefinition,
  FunctionParamDefinitions,
};

#endif