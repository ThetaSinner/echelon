#ifndef AST_NODE_HPP_INCLUDED
#define AST_NODE_HPP_INCLUDED

#include <vector>
#include <string>

#include <echelon/ast/ast-node-type-enum.hpp>

class AstNode {
  AstNodeType type;
  std::string data;
  std::vector<AstNode *> children;
public:
  int getChildCount();

  void putChild(AstNode *child);

  void putChildFront(AstNode *child);

  AstNode *getChild(unsigned index);

  bool hasChild(AstNodeType type);

  AstNode *getChild(AstNodeType type);

  void setType(AstNodeType type);

  AstNodeType getType();

  void setData(std::string data);

  std::string getData();
};

#endif
