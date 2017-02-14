#ifndef CONTEXT_ITEM_HPP_INCLUDED
#define CONTEXT_ITEM_HPP_INCLUDED

#include <string>

/**
 * Single item in a context list.
 * For example, store "MyModule"
 */
class ContextItem {
  std::string name;

public:
  /**
   * Construct a context item with its name.
   *
   * @param name the name of the context item.
   */
  ContextItem(std::string name);

  /**
   * Set the context item name.
   *
   * @param name the context item name.
   */
  void setName(std::string name);

  /**
   * Get the context item name.
   *
   * @return the context item name.
   */
  std::string getName();
};

#endif
