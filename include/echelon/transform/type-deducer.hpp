#ifndef TYPE_DEDUCTION_ENGINE_HPP_INCLUDED
#define TYPE_DEDUCTION_ENGINE_HPP_INCLUDED

#include <string>

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/ast/ast-node.hpp>
#include <echelon/util/event/event-container.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/transform/type-rule-lookup.hpp>

class TypeDeducer {
  void resolveTypeFromExpression(EnhancedAstNode* expressionNode, Scope* scope);
public:
  /**
   * Deduce types from a tree of \sa{EnhancedAstNode}
   */
  void deduceTypes(EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target);

  std::string toTypeName(EnhancedAstNode* node);
};

#endif
