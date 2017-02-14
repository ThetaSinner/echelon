#ifndef AST_NODE_ENHANCER_OUTPUT_DATA_HPP_INCLUDED
#define AST_NODE_ENHANCER_OUTPUT_DATA_HPP_INCLUDED

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>
#include <echelon/transform/ast-node-enhancer-input-data.hpp>

class AstNodeEnhancerOutputData {
  AstNode *source;
  EnhancedAstNode *target;

public:
  AstNodeEnhancerOutputData(AstNodeEnhancerInputData& output);

  AstNode* getSourceNode();
  void setSourceNode(AstNode* source);

  EnhancedAstNode* getTargetNode();
  void setTargetNode(EnhancedAstNode* target);
};

#endif
