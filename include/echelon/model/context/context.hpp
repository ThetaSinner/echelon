#ifndef CONTEXT_HPP_INCLUDED
#define CONTEXT_HPP_INCLUDED

#include <list>

#include <echelon/model/context/context-item.hpp>

/**
 * Container for a list of context items.
 * For example, store ["MyModule", "MyType", "myVar"]
 *
 * The context should be attached to each scope level (scope on block node).
 * Also to each element which needs a context path (variable, function, logic statement).
 */
class Context {
  Context* parentContext;
  ContextItem* contextItem;

public:
  /**
   * Construct a context.
   *
   * @param parentContext The parent context or a null pointer.
   * @param contextItem The context item for this context.
   */
  Context(Context* parentContext, ContextItem* contextItem);

  /**
   * Get the context item.
   *
   * @return the context item.
   */
  ContextItem* getContextItem();

  /**
   * Get the parent context.
   *
   * @return the parent context.
   */
  Context* getParentContext();

  /**
   * Convert the context to a string representation.
   *
   * @param recursive whether the call toString is recursive.
   * @return the string representation of the context.
   */
  std::string toString(bool recursive = false);
};

#endif
