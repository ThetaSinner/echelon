#ifndef NAME_RESOLVER_HPP_INCLUDED
#define NAME_RESOLVER_HPP_INCLUDED

#include <list>
#include <queue>

#include <echelon/model/internal/enhanced-ast-block-node.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>

class NameResolver {
  std::queue<std::string> toNameStructure(EnhancedAstNode* node);
  std::queue<std::string> toNameStructureFromNameStructureNode(EnhancedAstNode* node);

  EnhancedAstNode* resolveFromNameStructure(EnhancedAstNode* unresolved, std::queue<std::string> nameStructure, Scope* scope);
public:
  EnhancedAstNode* resolve(EnhancedAstNode* unresolved, Scope* scope);
};

#endif
