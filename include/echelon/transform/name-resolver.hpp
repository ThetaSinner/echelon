#ifndef NAME_RESOLVER_HPP_INCLUDED
#define NAME_RESOLVER_HPP_INCLUDED

#include <list>
#include <queue>

#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

class NameResolver {
  std::queue<std::string> toNameStructure(EnhancedAstNode* node);
  std::queue<std::string> toNameStructureFromNameStructureNode(EnhancedAstNode* node);

  EnhancedAstNode* resolveInternal(EnhancedAstNode* unresolved, std::queue<std::string> nameStructure, Scope* scope);
public:
  EnhancedAstNode* resolve(EnhancedAstNode* unresolved, Scope* scope);
};

#endif
