#ifndef ENHANCED_AST_NODE_HPP_INCLUDED
#define ENHANCED_AST_NODE_HPP_INCLUDED

#include <vector>
#include <string>

#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

#ifdef ECHELON_DEBUG

#include <iostream>
#include <echelon/util/echelon-lookup.hpp>

#endif

/**
 * \class
 */
class EnhancedAstNode {
  EnhancedAstNodeType nodeType = EnhancedAstNodeType::None;
  EnhancedAstNodeSubType nodeSubType = EnhancedAstNodeSubType::Unspecified;
  std::string data;

  std::vector<EnhancedAstNode *> childList;
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
    this->data = data;
  }

  std::string &getData() {
    return data;
  }

  void putChild(EnhancedAstNode *child) {
    childList.push_back(child);
  }

  void putChildFront(EnhancedAstNode *child) {
    childList.insert(childList.begin(), child);
  }

  int getChildCount() {
    return childList.size();
  }

  std::vector<EnhancedAstNode *> *getChildList() {
    return &childList;
  }

  EnhancedAstNode *getChild(int childIndex) {
    return childList.at(childIndex);
  }

  bool hasChild(EnhancedAstNodeType childType) {
    for (auto i : childList) {
      if (i->getNodeType() == childType) {
        return true;
      }
    }

    return false;
  }

  void removeChild(EnhancedAstNode* enhancedAstNode) {
    for (auto c = childList.begin(); c != childList.end(); c++) {
      if (&**c == enhancedAstNode) {
        childList.erase(c);
        break; // have to break or call method recursively because the iterator is destroyed.
      }
    }
  }

  EnhancedAstNode *getChild(EnhancedAstNodeType childType) {
    for (auto i : childList) {
      if (i->getNodeType() == childType) {
        return i;
      }
    }

#ifdef ECHELON_DEBUG
    std::string message =
        "Request for child by childType [" + EchelonLookup::getInstance()->toString(childType) + "] failed.";
    throw std::runtime_error(message.c_str());
#else
    return nullptr;
#endif
  }

  EnhancedAstNode* getLastChild() {
    return childList.at(childList.size() - 1);
  }
};

#endif
