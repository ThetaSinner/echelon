#ifndef AST_ENHANCER_HELPER_HPP_INCLUDED
#define AST_ENHANCER_HELPER_HPP_INCLUDED

#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>

class AstEnhancerHelper {
public:
  static void mapBlockIfPresent(AstNode* nodeToMap, EnhancedAstNode* target, const AstNodeEnhancerInputData& input);
};

#endif
