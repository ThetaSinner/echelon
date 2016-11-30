#include <echelon/parser/stage2/ast-construction-manager.hpp>

#ifdef ECHELON_DEBUG
#include <stdexcept>
#include <iostream>
#endif

AstConstructionManager::AstConstructionManager() {
  root = new AstNode();
  root -> setData("root"); // use project name?
  root -> setType(AstNodeType::Program);
  workingNode = root;
}

void AstConstructionManager::pushFragment(AstNode* fragment) {
  #ifdef ECHELON_DEBUG
  if (fragment == nullptr) {
    std::cout << "Attempt to push bad fragment." << std::endl;
    //throw std::runtime_error("Attempt to push bad fragment.");
  }
  #endif

  if (fragment -> getType() == AstNodeType::Program && fragment -> getChildCount() > 0) {
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
