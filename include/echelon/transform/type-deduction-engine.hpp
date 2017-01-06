#ifndef TYPE_DEDUCTION_ENGINE_HPP_INCLUDED
#define TYPE_DEDUCTION_ENGINE_HPP_INCLUDED

#include <echelon/ast/ast-node.hpp>

class TypeDeductionEngine {
public:
  void deduceTypes(AstNode *root);
};

#endif
