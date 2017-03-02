#ifndef ENHANCED_AST_BLOCK_NODE_HPP_INCLUDED
#define ENHANCED_AST_BLOCK_NODE_HPP_INCLUDED

#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>

/**
 * Customised internal model node which can store extra information required at the top level of a block.
 */
class EnhancedAstBlockNode : public EnhancedAstNode {
  Scope* scope;
  EnhancedAstNode* owner;

public:
  /**
   * Set the scope.
   *
   * @param scope the scope.
   */
  void setScope(Scope* scope);
  /**
   * Get the scope.
   *
   * @return the scope.
   */
  Scope* getScope();

  /**
   * Set the owner of this block node.
   *
   * @param owner the owner.
   */
  void setOwner(EnhancedAstNode* owner) {
    this->owner;
  }

  /**
   * Get the owner of this block node.
   *
   * @return the owner.
   */
  EnhancedAstNode* getOwner() {
    return owner;
  }
};

#endif
