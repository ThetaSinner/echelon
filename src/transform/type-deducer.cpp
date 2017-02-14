#include <echelon/transform/type-deducer.hpp>

NameResolver TypeDeducer::nameResolver;

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

  // Fine to resolve to the type of an expression, but we don't care about the node itself, start from its data.
  if (expressionNode->getNodeType() == EnhancedAstNodeType::Expression) {
    return resolveTypeFromExpression(expressionNode->getChild(0), scope);
  }

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
    auto typeNameResolve = resolveTypeName(expressionNode, scope);

    if (typeNameResolve.isResolved()) {
      typeResolve.setTypeName(typeNameResolve.getTypeName());
    }
    // else map dependencies.
  }

  return typeResolve;
}

TypeNameResolve TypeDeducer::resolveTypeName(EnhancedAstNode* node, Scope* scope) {
  TypeNameResolve typeNameResolve;

  if (node->getNodeType() == EnhancedAstNodeType::PrimitiveValue) {
    switch (node->getNodeSubType()) {
      case EnhancedAstNodeSubType::Integer:
        typeNameResolve.setTypeName("integer");
        break;
      case EnhancedAstNodeSubType::Decimal:
        typeNameResolve.setTypeName("decimal");
        break;
      default:
        throw std::runtime_error("Unrecognised primitive value type");
    }
  }
  else if (node->getNodeType() == EnhancedAstNodeType::VariableValue) {
    auto var = nameResolver.resolve(node, scope);

    if (var != nullptr) {
      if (var->hasChild(EnhancedAstNodeType::TypeName)) {
        typeNameResolve.setTypeName(var->getChild(EnhancedAstNodeType::TypeName)->getData());
      }
      else {
        // Missing type name.
        // TODO listen for var to change.
        throw std::runtime_error("Depends on variable with missing type name.");
      }
    }
    else {
      // Variable doesn't exist.
      throw std::runtime_error("Depends on variable which doesn't exist [" + node->getData() + "].");
    }
  }
  else if (node->getNodeType() == EnhancedAstNodeType::AccessExpression) {
    auto ae = nameResolver.resolve(node, scope);

    if (ae != nullptr) {
      if (ae->hasChild(EnhancedAstNodeType::TypeName)) {
        typeNameResolve.setTypeName(ae->getChild(EnhancedAstNodeType::TypeName)->getData());
      }
      else {
        // Missing type name.
        throw std::runtime_error("Access expression does not have a type name");
      }
    }
    else {
      // Does not refer to a valid element.
      throw std::runtime_error("Invalid access expression [" + to_string(node) + "]");
    }
  }
  else {
    throw std::runtime_error("failure: type resolution requested for unhandled node type");
  }

  return typeNameResolve;
}