#ifndef AST_TO_GRAPHVIZ_HPP_INCLUDED
#define AST_TO_GRAPHVIZ_HPP_INCLUDED

#include <string>
#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

std::string toGraphviz(AstNode *program);

std::string toGraphviz(EnhancedAstNode* program);

#endif
