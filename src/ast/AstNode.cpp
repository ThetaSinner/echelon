#include <echelon/ast/AstNode.hpp>

void AstNode::putChild(AstNode* child) {
  children.push_back(child);
}

void AstNode::putChildFront(AstNode* child) {
  children.insert(children.begin(), child);
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

void AstNode::setData(std::string data) {
  this -> data = data;
}
std::string AstNode::getData() {
  return data;
}
