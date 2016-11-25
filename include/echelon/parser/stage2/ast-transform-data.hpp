#ifndef AST_TRANSFORM_DATA_HPP_INCLUDED
#define AST_TRANSFORM_DATA_HPP_INCLUDED

#include <list>
#include <queue>

#include <echelon/ast/AstNode.hpp>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/parser/stage2/pattern-match-info.hpp>

class AstTransformData {
  std::list<EnhancedToken*> tokens;
  PatternMatchInfo* patternMatchInfo;

  std::queue<AstNode*>* subProcessAstNodes;
  std::queue<AstNode*>* nestedAstNodes;
public:
  void setTokens(std::list<EnhancedToken*> tokens) {
    this -> tokens = tokens;
  }
  std::list<EnhancedToken*>* getTokens() {
    return &tokens;
  }

  void setPatternMatchInfo(PatternMatchInfo* patternMatchInfo) {
    this -> patternMatchInfo = patternMatchInfo;
  }
  PatternMatchInfo* getPatternMatchInfo() {
    return patternMatchInfo;
  }

  void setSubProcessAstNodes(std::queue<AstNode*>* subProcessAstNodes) {
    this -> subProcessAstNodes = subProcessAstNodes;
  }
  std::queue<AstNode*>* getSubProcessAstNodes() {
    return subProcessAstNodes;
  }

  void setNestedAstNodes(std::queue<AstNode*>* nestedAstNodes) {
    this -> nestedAstNodes = nestedAstNodes;
  }
  std::queue<AstNode*>* getNestedAstNodes() {
    return nestedAstNodes;
  }
};

#endif
