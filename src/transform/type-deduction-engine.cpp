#include <echelon/transform/type-deduction-engine.hpp>

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/parser/echelon-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>

void TypeDeductionEngine::deduceTypes(AstNode* root) {
  // Found variable declaration.
  if (root -> getType() == AstNodeType::Variable) {
    // Variable doesn't have type information.
    if (root -> getChild(0) -> getType() != AstNodeType::Type) {
      // As soon as it's compound, or refers to another variable, or calls a function we need to do a lot more work.
      // Thus, the example here seems so trivial as to be counterproductive, but having a uniform tree in more complex cases
      // will be invaluable. (I hope);
      if (root -> getChild(0) -> getType() == AstNodeType::String) {
        AstNode *typeNode = new AstNode();
        typeNode -> setType(AstNodeType::Type);
        typeNode -> setData(EchelonLookup::getInstance() -> toString(Keyword::String));

        root -> putChildFront(typeNode);
      }
    }
  }

  for (int i = 0; i < root -> getChildCount(); i++) {
    deduceTypes(root -> getChild(0));
  }
}
