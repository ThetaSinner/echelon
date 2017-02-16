#ifndef ENHANCED_AST_CONTEXT_AWARE_NODE_HPP_INCLUDED
#define ENHANCED_AST_CONTEXT_AWARE_NODE_HPP_INCLUDED

#include <echelon/model/context/context.hpp>

/**
 * Behaviour class to make internal model nodes context aware.
 * Not directly constructable, must be inherited.
 */
class EnhancedAstContextAwareNode {
  Context* context;

protected:
  EnhancedAstContextAwareNode() {}

public:
  /**
   * Set the context for this model node.
   *
   * @param context the context.
   */
  void setContext(Context* context) {
    this->context = context;
  }

  /**
   * Get the context for this model node.
   *
   * @return the context.
   */
  Context* getContext() {
    return context;
  }
};

#endif
