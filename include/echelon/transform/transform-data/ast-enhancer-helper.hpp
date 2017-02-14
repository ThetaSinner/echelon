#ifndef AST_ENHANCER_HELPER_HPP_INCLUDED
#define AST_ENHANCER_HELPER_HPP_INCLUDED

#include <string>
#include <sstream>

#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/internal/node-enhancer-lookup.hpp>

class AstEnhancerHelper {
public:
  static void mapBlockIfPresent(AstNode* nodeToMap, EnhancedAstNode* target, const AstNodeEnhancerInputData& input);

  static void simpleMapNode(AstNode* nodeToMap, EnhancedAstNode* target, AstNodeEnhancerInputData& input);

  static bool doFunctionSignaturesMatch(EnhancedAstNode *left, EnhancedAstNode *right);

  static std::string computeFunctionHash(EnhancedAstNode* enhancedAstNode);

  static void mapChildIfPresent(EnhancedAstNode* target, AstNode* nodeToMap, const AstNodeEnhancerInputData& input, AstNodeType astNodeType);
};

#endif
