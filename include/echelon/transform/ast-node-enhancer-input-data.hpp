#ifndef AST_NODE_ENHANCER_INPUT_HPP_INCLUDED
#define AST_NODE_ENHANCER_INPUT_HPP_INCLUDED

#include <echelon/ast/ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

class AstNodeEnhancerInputData {
  AstNode *source;
  AstNode *nodeToMap;
  EnhancedAstNode *target;
  Scope *scope;

public:
  AstNode* getSourceNode();
  void setSourceNode(AstNode* source);

  AstNode* getNodeToMap();
  void setNodeToMap(AstNode* nodeToMap);

  EnhancedAstNode* getTargetNode();
  void setTargetNode(EnhancedAstNode* target);

  Scope* getScope();
  void setScope(Scope* scope);
};

#endif
