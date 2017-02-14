#ifndef AST_ENHANCER_HPP_INCLUDED
#define AST_ENHANCER_HPP_INCLUDED

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>

class AstEnhancer {
public:
  EnhancedAstNode *enhance(AstNode *node);
};

#endif
