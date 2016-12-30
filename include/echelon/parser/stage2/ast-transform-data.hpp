#ifndef AST_TRANSFORM_DATA_HPP_INCLUDED
#define AST_TRANSFORM_DATA_HPP_INCLUDED

#include <list>
#include <queue>

#include <echelon/ast/ast-node.hpp>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/parser/stage2/pattern-match-info.hpp>

class AstTransformData {
  std::list<EnhancedToken*> tokens;
  PatternMatchInfo* patternMatchInfo;

  std::queue<AstNode*>* subProcessAstNodes;
  std::queue<AstNode*>* nestedAstNodes;
public:
  void setTokens(std::list<EnhancedToken*> tokens);
  std::list<EnhancedToken*>* getTokens();

  void setPatternMatchInfo(PatternMatchInfo* patternMatchInfo);
  PatternMatchInfo* getPatternMatchInfo();

  void setSubProcessAstNodes(std::queue<AstNode*>* subProcessAstNodes);
  std::queue<AstNode*>* getSubProcessAstNodes();

  void setNestedAstNodes(std::queue<AstNode*>* nestedAstNodes);
  std::queue<AstNode*>* getNestedAstNodes();
};

#endif
