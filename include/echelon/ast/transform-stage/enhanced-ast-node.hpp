#ifndef ENHANCED_AST_NODE_HPP_INCLUDED
#define ENHANCED_AST_NODE_HPP_INCLUDED

#include <list>

#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

class EnhancedAstNode {
  EnhancedAstNodeType nodeType = EnhancedAstNodeType::None;
  EnhancedAstNodeSubType nodeSubType = EnhancedAstNodeSubType::Unspecified;

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

  void putChild(EnhancedAstNode* child) {
    childList.push_back(child);
  }
};

#endif
