#ifndef AST_NODE_HPP_INCLUDED
#define AST_NODE_HPP_INCLUDED

#include <elm/ast/AstNodeType.hpp>
#include <vector>

class AstNode {
private:
  AstNodeType type;
  std::vector<AstNode*> children;
public:
  int getChildCount();
  
  void putChild(AstNode* child);
  AstNode* getChild(int index);
  
  void setType(AstNodeType type);
  AstNodeType getType();
};

#endif
