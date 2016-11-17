#include <echelon/parser/stage2/ast-transform.hpp>

AstTransform::AstTransform(std::function<AstNode*(AstTransformData*)> transformer) {
  this -> transformer = transformer;
}

AstNode* AstTransform::transform(AstTransformData* astTransformData) {
  return transformer(astTransformData);
}
