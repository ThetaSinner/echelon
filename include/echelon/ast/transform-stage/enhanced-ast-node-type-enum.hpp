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

  Function,
  FunctionParamDefinition,
  FunctionParamDefinitions,

  SingleLineComment,

#ifdef ECHELON_DEBUG
  First = None,
  Last = SingleLineComment,
#endif
};

#endif