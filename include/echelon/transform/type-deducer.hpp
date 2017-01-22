#ifndef TYPE_DEDUCTION_ENGINE_HPP_INCLUDED
#define TYPE_DEDUCTION_ENGINE_HPP_INCLUDED

#include <string>

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/ast/ast-node.hpp>
#include <echelon/util/event/event-container.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/transform/type-rule-lookup.hpp>

class TypeResolve {
  std::string typeName;
  bool resolved = false;

  std::list<EnhancedAstNode*> dependencyNodes;
public:
  void setTypeName(std::string typeName) {
    this->typeName = typeName;
  }

  std::string getTypeName() {
    return typeName;
  }

  bool isResolved() {
    return resolved;
  }

  void pushDependency(EnhancedAstNode* node) {
    dependencyNodes.push_back(node);
  }
};

class TypeNameResolve {
  std::string typeName;
  bool resolved = false;

public:
  void setTypeName(std::string typeName) {
    this->typeName = typeName;
    resolved = true;
  }

  std::string getTypeName() {
    return typeName;
  }

  bool isResolved() {
    return resolved;
  }
};

class TypeDeducer {
  TypeResolve resolveTypeFromExpression(EnhancedAstNode* expressionNode, Scope* scope);
public:
  /**
   * Deduce types from a tree of \sa{EnhancedAstNode}
   */
  void deduceTypes(EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target);

  TypeNameResolve resolveTypeName(EnhancedAstNode* node);
};

#endif
