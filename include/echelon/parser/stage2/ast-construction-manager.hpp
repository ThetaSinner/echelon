#ifndef AST_CONSTRUCTION_MANAGER_HPP_INCLUDED
#define AST_CONSTRUCTION_MANAGER_HPP_INCLUDED

#include <echelon/ast/AstNode.hpp>
#include <echelon/ast/AstNodeType.hpp>

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
