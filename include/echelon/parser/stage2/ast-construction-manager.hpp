#ifndef AST_CONSTRUCTION_MANAGER_HPP_INCLUDED
#define AST_CONSTRUCTION_MANAGER_HPP_INCLUDED

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/ast/ast-node-type-enum.hpp>

class AstConstructionManager {
  AstNode *root;
  AstNode *workingNode;

  bool anyFragmentsPushed = false;
public:
  AstConstructionManager();

  void pushFragment(AstNode *fragment);

  AstNode *getRoot();

  bool areAnyFragmentsPushed();
};

#endif
