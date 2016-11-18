#include <echelon/parser/stage2/ast-construction-manager.hpp>

AstConstructionManager::AstConstructionManager() {
  root = new AstNode();
  root -> setData("root"); // use project name?
  root -> setType(AstNodeType::Program);
  workingNode = root;
}

void AstConstructionManager::pushFragment(AstNode* fragment) {
  // handle special cases

  workingNode -> putChild(fragment);
}

AstNode* AstConstructionManager::getRoot() {
  return root;
}
