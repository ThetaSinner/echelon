#ifndef OPERATOR_PRECEDENCE_TREE_RESTRUCTURER_HPP_INCLUDED
#define OPERATOR_PRECEDENCE_TREE_RESTRUCTURER_HPP_INCLUDED

#include <list>

#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

class OperatorPrecedenceTreeRestructurer {
  static EnhancedAstNodeSubType nextOperator(EnhancedAstNodeSubType astNodeType);
  static EnhancedAstNode* restructureInternal(EnhancedAstNode* node, EnhancedAstNodeSubType nodeSubType);
public:
  static EnhancedAstNode* restructure(EnhancedAstNode *node);
};

#endif