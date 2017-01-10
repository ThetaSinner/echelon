#ifndef ENHANCED_AST_NODE_TYPE_ENUM_HPP_INCLUDED
#define ENHANCED_AST_NODE_TYPE_ENUM_HPP_INCLUDED

enum class EnhancedAstNodeType {
  None,

  Package,

  Program,
  Variable,
  PrimitiveValue,
  TypeName,

  Function,
  FunctionParamDefinition,
  FunctionParamDefinitions,

#ifdef ECHELON_DEBUG
  First = None,
  Last = FunctionParamDefinitions,
#endif
};

#endif