#include <echelon/parser/stage2/ast-construction-manager.hpp>

#include <stdexcept>
#include <echelon/util/logging/logger-shared-instance.hpp>

AstConstructionManager::AstConstructionManager() {
  root = new AstNode();
  root -> setData("root"); // TODO use project name?
  root -> setType(AstNodeType::Program);
  workingNode = root;
}

void AstConstructionManager::pushFragment(AstNode* fragment) {
  #ifdef ECHELON_DEBUG
  if (fragment == nullptr) {
    LoggerSharedInstance::get() -> at(Level::Warn) << "Attempt to push bad fragment.\n";
    throw std::runtime_error("Attempt to push bad fragment.");
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
