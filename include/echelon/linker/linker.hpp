#ifndef LINKER_HPP_INCLUDED
#define LINKER_HPP_INCLUDED

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>
#include <echelon/ast/scope/scope-helper.hpp>
#include <echelon/ast/scope/scope-pusher.hpp>

class Scope;
#include <echelon/ast/scope/scope.hpp>

/**
 * Linker for echelon programs.
 */
class Linker {

public:
  /**
   * Link a partial to a target node.
   * For example inject a create method (partial) onto a custom type (target)
   *
   * @param target The target to link to.
   * @param partial The partial to link.
   */
  void linkPartial(EnhancedAstNode* target, EnhancedAstNode* partial);
};

#endif
