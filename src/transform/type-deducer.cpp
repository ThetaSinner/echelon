#include <echelon/transform/type-deducer.hpp>

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

void TypeDeducer::deduceTypes(EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target) {
  // need a node to work from. expression or bool expr etc.
  // scope pointer,
  // a target to map the type onto

  resolveTypeFromExpression(expressionNode, scope);
  // register listeners for each type we don't have.
  // if all listeners complete then figure out this type.

  // if no listeners are registered then trigger event for this type completing.
  // it's fine to send the event, anything waiting for this type will complete and anything which hasn't registered a listener yet
  // will just be able to resolve this type immediately. To parallelize all types should resolve and none should be triggered until
  // this method won't be called again.
}

TypeResolve TypeDeducer::resolveTypeFromExpression(EnhancedAstNode* expressionNode, Scope* scope) {
  // grab operator and call resolve on left and right.
  // use rules to determine the result i.e. *, integer, decimal -> decimal
  // either return the type or an object describing what's missing.

  TypeResolve typeResolve;

  if (expressionNode->getChildCount() > 0) {
    auto typeNameResolveLeft = resolveTypeFromExpression(expressionNode->getChild(0), scope);
    auto typeNameResolveRight = resolveTypeFromExpression(expressionNode->getChild(1), scope);

    if (typeNameResolveLeft.isResolved() && typeNameResolveRight.isResolved()) {
      if (TypeRuleLookup::hasRule(expressionNode->getNodeSubType(), typeNameResolveLeft.getTypeName(), typeNameResolveRight.getTypeName())) {
        typeResolve.setTypeName(TypeRuleLookup::lookup(expressionNode->getNodeSubType(), typeNameResolveLeft.getTypeName(), typeNameResolveRight.getTypeName()));
      }
      // else error.
    }
    // else map dependencies.
  }
  else {
    auto typeNameResolve = resolveTypeName(expressionNode);

    if (typeNameResolve.isResolved()) {
      typeResolve.setTypeName(typeNameResolve.getTypeName());
    }
    // else map dependencies.
  }

  return typeResolve;
}

TypeNameResolve TypeDeducer::resolveTypeName(EnhancedAstNode* node) {
  TypeNameResolve typeNameResolve;

  if (node->getNodeType() == EnhancedAstNodeType::PrimitiveValue) {
    switch (node->getNodeSubType()) {
      case EnhancedAstNodeSubType::Integer:
        typeNameResolve.setTypeName("integer");
    }
  }

  return typeNameResolve;
}