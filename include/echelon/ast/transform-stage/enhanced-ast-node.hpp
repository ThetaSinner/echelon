#ifndef ENHANCED_AST_NODE_HPP_INCLUDED
#define ENHANCED_AST_NODE_HPP_INCLUDED

#include <list>
#include <string>

#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <echelon/parser/echelon-lookup.hpp>
#endif

class EnhancedAstNode {
  EnhancedAstNodeType nodeType = EnhancedAstNodeType::None;
  EnhancedAstNodeSubType nodeSubType = EnhancedAstNodeSubType::Unspecified;
  std::string data;

  std::vector<EnhancedAstNode*> childList;
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

  std::string& getData() {
    return data;
  }

  void putChild(EnhancedAstNode* child) {
    childList.push_back(child);
  }

  int getChildCount() {
    return childList.size();
  }

  std::vector<EnhancedAstNode*>* getChildList() {
    return &childList;
  }

  EnhancedAstNode* getChild(int childIndex) {
    return childList.at(childIndex);
  }

  bool hasChild(EnhancedAstNodeType childType) {
    for (auto i : childList) {
      if (i -> getNodeType() == childType) {
        return true;
      }
    }

    return false;
  }

  EnhancedAstNode* getChild(EnhancedAstNodeType childType) {
    for (auto i : childList) {
      if (i -> getNodeType() == childType) {
        return i;
      }
    }

    #ifdef ECHELON_DEBUG
    std::string message = "Request for child by childType [" + EchelonLookup::getInstance() -> toString(childType) + "] failed.";
    throw std::runtime_error(message.c_str());
    #else
    return nullptr;
    #endif
  }
};

#endif
