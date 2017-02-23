#ifndef AST_ENHANCER_OUTPUT_HPP_INCLUDED
#define AST_ENHANCER_OUTPUT_HPP_INCLUDED

#include <echelon/transform/transform-working-data.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>

/**
 * Output from the ast enhancer.
 */
class AstEnhancerOutput {
  EnhancedAstNode* enhancedAstNode;
  TransformWorkingData* transformWorkingData;

public:
  /**
   * Set the enhanced ast node.
   *
   * @param enhancedAstNode the enhanced ast node.
   */
  void setEnhancedAstNode(EnhancedAstNode* enhancedAstNode) {
    this->enhancedAstNode = enhancedAstNode;
  }

  /**
   * Get the enhanced ast node.
   *
   * @return the enhanced ast node.
   */
  EnhancedAstNode* getEnhancedAstNode() {
    return enhancedAstNode;
  }

  /**
   * Set the transform working data.
   *
   * @param transformWorkingData the transform working data.
   */
  void setTransformWorkingData(TransformWorkingData* transformWorkingData) {
    this->transformWorkingData = transformWorkingData;
  }

  /**
   * Get the transform working data.
   *
   * @return the transform working data.
   */
  TransformWorkingData* getTransformWorkingData() {
    return transformWorkingData;
  }
};

#endif
