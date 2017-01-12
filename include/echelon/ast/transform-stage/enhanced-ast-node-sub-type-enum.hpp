#ifndef ENHANCED_AST_NODE_SUB_TYPE_ENUM_HPP_INCLUDED
#define ENHANCED_AST_NODE_SUB_TYPE_ENUM_HPP_INCLUDED

enum class EnhancedAstNodeSubType {
  Unspecified,

  Declaration,
  Assign,

  // Primitive Value
      Integer,

  // Type
      IntegerType,

  // Function
  Implementation,
  Prototype,

  // Binary operator
  Multiply,

#ifdef ECHELON_DEBUG
  First = Unspecified,
  Last = IntegerType,
#endif
};

#endif
