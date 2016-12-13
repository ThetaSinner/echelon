#ifndef AST_TRANSFORM_HPP_INCLUDED
#define AST_TRANSFORM_HPP_INCLUDED

#include <functional>

#include <echelon/ast/ast-node.hpp>
#include <echelon/parser/stage2/ast-transform-data.hpp>

class AstTransform {
  std::function<AstNode*(AstTransformData*)> transformer;

public:
  AstTransform(std::function<AstNode*(AstTransformData*)> transformer);

  AstNode* transform(AstTransformData* astTransformData);
};

#endif
