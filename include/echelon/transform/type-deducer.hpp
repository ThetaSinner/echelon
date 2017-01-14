#ifndef TYPE_DEDUCTION_ENGINE_HPP_INCLUDED
#define TYPE_DEDUCTION_ENGINE_HPP_INCLUDED

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/ast/ast-node.hpp>
#include <echelon/util/event/event-container.hpp>

class TypeDeducer {
public:
  void deduceTypes(AstNode *root);
};

#endif
