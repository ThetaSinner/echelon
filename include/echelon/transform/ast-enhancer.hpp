#ifndef AST_ENHANCER_HPP_INCLUDED
#define AST_ENHANCER_HPP_INCLUDED

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/transform/ast-enhancer-output.hpp>

class AstEnhancer {
public:
  AstEnhancerOutput* enhance(AstNode *node);
};

#endif
