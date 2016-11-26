#include <echelon/parser/stage2/ast-construction-manager.hpp>

AstConstructionManager::AstConstructionManager() {
  root = new AstNode();
  root -> setData("root"); // use project name?
  root -> setType(AstNodeType::Program);
  workingNode = root;
}

void AstConstructionManager::pushFragment(AstNode* fragment) {
  if (fragment -> getType() == AstNodeType::Program) {
    workingNode -> putChild(fragment -> getChild(0));
  }
  else {
    workingNode -> putChild(fragment);
  }
  anyFragmentsPushed = true;
}

AstNode* AstConstructionManager::getRoot() {
  return root;
}

bool AstConstructionManager::areAnyFragmentsPushed() {
  return anyFragmentsPushed;
}
