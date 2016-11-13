#ifndef AST_NODE_HPP_INCLUDED
#define AST_NODE_HPP_INCLUDED

#include <echelon/ast/AstNodeType.hpp>
#include <vector>
#include <string>

class AstNode {
private:
  AstNodeType type;
  std::string data;
  std::vector<AstNode*> children;
public:
  int getChildCount();

  void putChild(AstNode* child);
  AstNode* getChild(int index);

  void setType(AstNodeType type);
  AstNodeType getType();

  void setData(std::string data);
  std::string getData();
};

#endif
