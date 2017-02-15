#ifndef AST_NODE_ENHANCER_INPUT_HPP_INCLUDED
#define AST_NODE_ENHANCER_INPUT_HPP_INCLUDED

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>
#include <echelon/model/context/context.hpp>

class AstNodeEnhancerInputData {
  AstNode *source;
  AstNode *nodeToMap;
  EnhancedAstNode *target;
  Scope *scope;
  Context* context;

public:
  AstNode* getSourceNode();
  void setSourceNode(AstNode* source);

  AstNode* getNodeToMap();
  void setNodeToMap(AstNode* nodeToMap);

  EnhancedAstNode* getTargetNode();
  void setTargetNode(EnhancedAstNode* target);

  Scope* getScope();
  void setScope(Scope* scope);

  /**
   * Set a new context to be passed to the start of a new scope.
   *
   * @param context the updated context.
   */
  void setUpdatedContext(Context* context);

  /**
   * Get the updated context to be used on a new scope.
   *
   * @return the updated context.
   */
  Context* getUpdatedContext();
};

#endif
