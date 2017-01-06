#ifndef AST_ENHANCER_HPP_INCLUDED
#define AST_ENHANCER_HPP_INCLUDED

#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

class AstEnhancer {
public:
  EnhancedAstNode *enhance(AstNode *node);
};

#endif
