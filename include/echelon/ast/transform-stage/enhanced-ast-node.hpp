#ifndef ENHANCED_AST_NODE_HPP_INCLUDED
#define ENHANCED_AST_NODE_HPP_INCLUDED

#include <list>
#include <string>

#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

class EnhancedAstNode {
  EnhancedAstNodeType nodeType = EnhancedAstNodeType::None;
  EnhancedAstNodeSubType nodeSubType = EnhancedAstNodeSubType::Unspecified;
  std::string data;

  std::list<EnhancedAstNode*> childList;
public:
  EnhancedAstNodeType getNodeType() const {
    return nodeType;
  }

  void setNodeType(EnhancedAstNodeType nodeType) {
    EnhancedAstNode::nodeType = nodeType;
  }

  EnhancedAstNodeSubType getNodeSubType() const {
    return nodeSubType;
  }

  void setNodeSubType(EnhancedAstNodeSubType nodeSubType) {
    EnhancedAstNode::nodeSubType = nodeSubType;
  }

  void setData(std::string data) {
    this -> data = data;
  }

  std::string getData() {
    return data;
  }

  void putChild(EnhancedAstNode* child) {
    childList.push_back(child);
  }

  std::list<EnhancedAstNode*>* getChildList() {
    return &childList;
  }
};

#endif
