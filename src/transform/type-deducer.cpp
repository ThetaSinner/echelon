#include <echelon/transform/type-deducer.hpp>

void TypeDeducer::deduceTypes(EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target) {
  // need a node to work from. expression or bool expr etc.
  // scope pointer,
  // a target to map the type onto

  auto typeResolve = resolveTypeFromExpression(expressionNode, scope);
  // register listeners for each type we don't have.
  // if all listeners complete then figure out this type.

  // if no listeners are registered then trigger event for this type completing.
  // it's fine to send the event, anything waiting for this type will complete and anything which hasn't registered a listener yet
  // will just be able to resolve this type immediately. To parallelize all types should resolve and none should be triggered until
  // this method won't be called again.

  if (typeResolve.isResolved()) {
    auto resolvedTypeName = typeResolve.getTypeName();

    if (target->hasChild(EnhancedAstNodeType::TypeName)) {
      auto& data = target->getChild(EnhancedAstNodeType::TypeName)->getData();

      if (resolvedTypeName != data) {
        throw std::runtime_error("attempt to assign type [" + resolvedTypeName + "] to type [" + data + "]");
        // TODO type casting.
      }
    }
    else {
      EnhancedAstNode* typeName = new EnhancedAstNode();
      typeName->setNodeType(EnhancedAstNodeType::TypeName);
      typeName->setData(resolvedTypeName);
      target->putChild(typeName);
    }

    // TODO trigger events.
  }
  else {
    throw std::runtime_error("type name cannot be resolved immediately, this is not supported");
  }
}

TypeResolve TypeDeducer::resolveTypeFromExpression(EnhancedAstNode* expressionNode, Scope* scope) {
  // grab operator and call resolve on left and right.
  // use rules to determine the result i.e. *, integer, decimal -> decimal
  // either return the type or an object describing what's missing.

  TypeResolve typeResolve;

  if (expressionNode->getChildCount() == 2) {
    // Try to resolve the left and right children.
    auto typeNameResolveLeft = resolveTypeFromExpression(expressionNode->getChild(0), scope);
    auto typeNameResolveRight = resolveTypeFromExpression(expressionNode->getChild(1), scope);

    if (typeNameResolveLeft.isResolved() && typeNameResolveRight.isResolved()) {
      if (TypeRuleLookup::getInstance()->hasRule(expressionNode->getNodeSubType(), typeNameResolveLeft.getTypeName(), typeNameResolveRight.getTypeName())) {
        typeResolve.setTypeName(TypeRuleLookup::getInstance()->lookup(expressionNode->getNodeSubType(), typeNameResolveLeft.getTypeName(), typeNameResolveRight.getTypeName()));
      }
      else {
        // TODO error message.
        throw std::runtime_error("Missing type rule.");
      }
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