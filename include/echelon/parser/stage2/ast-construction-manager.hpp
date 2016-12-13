#ifndef AST_CONSTRUCTION_MANAGER_HPP_INCLUDED
#define AST_CONSTRUCTION_MANAGER_HPP_INCLUDED

#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/ast-node-type.hpp>

class AstConstructionManager {
  AstNode *root;
  AstNode *workingNode;

  bool anyFragmentsPushed = false;
public:
  AstConstructionManager();

  void pushFragment(AstNode* fragment);
  AstNode* getRoot();

  bool areAnyFragmentsPushed();
};

#endif
