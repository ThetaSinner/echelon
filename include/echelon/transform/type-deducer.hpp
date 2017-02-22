#ifndef TYPE_DEDUCTION_ENGINE_HPP_INCLUDED
#define TYPE_DEDUCTION_ENGINE_HPP_INCLUDED

#include <string>

#include <echelon/model/ast/ast-node-type-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/model/ast/ast-node.hpp>
#include <echelon/util/event/event-container.hpp>
#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/scope/scope.hpp>
#include <echelon/transform/type-rule-lookup.hpp>
#include <echelon/transform/name-resolver.hpp>
#include <echelon/util/to-string.hpp>
#include <echelon/transform/transform-working-data.hpp>
#include <echelon/model/internal/enhanced-ast-variable-node.hpp>
#include <echelon/transform/type-events.hpp>
#include <echelon/transform/type-event-data.hpp>

class TypeResolve {
  std::string typeName;
  bool resolved = false;

  std::list<EnhancedAstNode*> missingDependencyNodes;
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

  void pushMissingDependency(EnhancedAstNode* node) {
    missingDependencyNodes.push_back(node);
  }

  std::list<EnhancedAstNode*> getMissingDependencyNodes() {
    return missingDependencyNodes;
  }
};

class TypeNameResolve {
  std::string typeName;
  bool resolved = false;

  std::string contextPath;
  bool contextPathFound = false;

  EnhancedAstNode* refersToMissingNode;
  bool refersToMissing = false;

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

  void setContextPath(std::string contextPath) {
    this->contextPath = contextPath;
    contextPathFound = true;
  }

  std::string getContextPath() {
    return contextPath;
  }

  bool isContextPathFound() {
    return contextPathFound;
  }

  void setRefersToMissingNode(EnhancedAstNode* refersToMissingNode) {
    this->refersToMissingNode = refersToMissingNode;
    refersToMissing = true;
  }

  EnhancedAstNode* getRefersToMissingNode() {
    return refersToMissingNode;
  }

  bool isRefersToMissing() {
    return refersToMissing;
  }
};

class TypeDeducer {
  static NameResolver nameResolver;

  static TypeResolve resolveTypeFromExpression(EnhancedAstNode* expressionNode, Scope* scope, EventContainer* eventContainer);
  static TypeNameResolve resolveTypeName(EnhancedAstNode* node, Scope* scope);
  static void mapTypeName(EnhancedAstNode* target, std::string data);
public:

  /**
   * Deduce types from a tree of \sa{EnhancedAstNode}
   */
  static void deduceTypes(EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target, TransformWorkingData* transformWorkingData);
};

#endif
