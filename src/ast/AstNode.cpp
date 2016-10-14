#include <elm/ast/AstNode.hpp>

void AstNode::putChild(AstNode* child) {
  children.push_back(child);
}

int AstNode::getChildCount() {
  return children.size();
}

AstNode* AstNode::getChild(int index) {
  return children.at(index);
}

void AstNode::setType(AstNodeType type) {
  this->type = type;
}

AstNodeType AstNode::getType() {
  return type;
}