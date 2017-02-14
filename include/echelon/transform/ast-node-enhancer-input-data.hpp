#ifndef AST_NODE_ENHANCER_INPUT_HPP_INCLUDED
#define AST_NODE_ENHANCER_INPUT_HPP_INCLUDED

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>

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
