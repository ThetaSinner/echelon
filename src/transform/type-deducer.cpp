#include <echelon/transform/type-deducer.hpp>

NameResolver TypeDeducer::nameResolver;

void TypeDeducer::deduceTypes(EnhancedAstNode* expressionNode, Scope* scope, EnhancedAstNode* target, TransformWorkingData* transformWorkingData) {
  // need a node to work from. expression or bool expr etc.
  // scope pointer,
  // a target to map the type onto

  auto typeResolve = resolveTypeFromExpression(expressionNode, scope, transformWorkingData);
  // register listeners for each type we don't have.
  // if all listeners complete then figure out this type.

  // if no listeners are registered then trigger event for this type completing.
  // it's fine to send the event, anything waiting for this type will complete and anything which hasn't registered a listener yet
  // will just be able to resolve this type immediately. To parallelize all types should resolve and none should be triggered until
  // this method won't be called again.

  if (typeResolve.isResolved()) {
    auto resolvedTypeName = typeResolve.getTypeName();
    TypeDeducer::mapTypeName(target, resolvedTypeName);

    // TODO trigger events.
  }
  else if (typeResolve.getMissingDependencyNodes().size()) {
    auto typeEventData = new TypeEventData();
    typeEventData->setEventGroupSize((int) typeResolve.getMissingDependencyNodes().size());

    for (auto& missing : typeResolve.getMissingDependencyNodes()) {
      TypeEvents::registerRefersToMissingFunction(missing, typeEventData, expressionNode, scope, target, transformWorkingData);
    }
  }
  /*else if () {
    eventContainer->addEventListener("type-name-added:" + ((EnhancedAstVariableNode*) var)->getContext()->toString(), [&eventContainer](EventKey& eventKey, void* data) -> void {
      std::cout << ((EnhancedAstNode*) data)->getData() << "\n";
      eventContainer->removeEventListener(eventKey);
    });
  }*/
  else {
    throw std::runtime_error("type name cannot be resolved immediately, this is not supported");
  }
}

TypeResolve TypeDeducer::resolveTypeFromExpression(EnhancedAstNode* expressionNode, Scope* scope, TransformWorkingData* transformWorkingData) {
  // grab operator and call resolve on left and right.
  // use rules to determine the result i.e. *, integer, decimal -> decimal
  // either return the type or an object describing what's missing.

  // Fine to resolve to the type of an expression, but we don't care about the node itself, start from its data.
  if (expressionNode->getNodeType() == EnhancedAstNodeType::Expression) {
    return resolveTypeFromExpression(expressionNode->getChild(0), scope, transformWorkingData);
  }

  TypeResolve typeResolve;

  if (expressionNode->getChildCount() == 2) {
    // Try to resolve the left and right children.
    auto typeResolveLeft = resolveTypeFromExpression(expressionNode->getChild(0), scope, transformWorkingData);
    auto typeResolveRight = resolveTypeFromExpression(expressionNode->getChild(1), scope, transformWorkingData);

    if (typeResolveLeft.isResolved() && typeResolveRight.isResolved()) {
      if (TypeRuleLookup::getInstance()->hasRule(expressionNode->getNodeSubType(), typeResolveLeft.getTypeName(), typeResolveRight.getTypeName())) {
        typeResolve.setTypeName(TypeRuleLookup::getInstance()->lookup(expressionNode->getNodeSubType(), typeResolveLeft.getTypeName(), typeResolveRight.getTypeName()));
      }
      else {
        // TODO error message.
        throw std::runtime_error("Missing type rule.");
      }
    }
    else {
      if (typeResolveLeft.getMissingDependencyNodes().size()) {
        for (auto i : typeResolveLeft.getMissingDependencyNodes()) {
          typeResolve.pushMissingDependency(i);
        }
      }
      if (typeResolveRight.getMissingDependencyNodes().size()) {
        for (auto i : typeResolveRight.getMissingDependencyNodes()) {
          typeResolve.pushMissingDependency(i);
        }
      }

      // TODO map context paths when node is found but has no type.
    }
  }
  else {
    auto typeNameResolve = resolveTypeName(expressionNode, scope, transformWorkingData);

    if (typeNameResolve.isResolved()) {
      typeResolve.setTypeName(typeNameResolve.getTypeName());
    }
    else if (typeNameResolve.isRefersToMissing()) {
      typeResolve.pushMissingDependency(typeNameResolve.getRefersToMissingNode());
    }
  }

  return typeResolve;
}

TypeNameResolve TypeDeducer::resolveTypeName(EnhancedAstNode* node, Scope* scope, TransformWorkingData* transformWorkingData) {
  TypeNameResolve typeNameResolve;

  if (node->getNodeType() == EnhancedAstNodeType::PrimitiveValue) {
    switch (node->getNodeSubType()) {
      case EnhancedAstNodeSubType::Integer:
        typeNameResolve.setTypeName("integer");
        break;
      case EnhancedAstNodeSubType::Decimal:
        typeNameResolve.setTypeName("decimal");
        break;
      case EnhancedAstNodeSubType::String:
        typeNameResolve.setTypeName("string");
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
        typeNameResolve.setContextPath(((EnhancedAstVariableNode*) var)->getContext()->toString());
      }
    }
    else {
      // Variable doesn't exist.
      // The compiler probably shouldn't try to solve this problem. It makes the language too complex.
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
  else if (node->getNodeType() == EnhancedAstNodeType::FunctionCall) {
    if (node->hasChild(EnhancedAstNodeType::FunctionCallParams)) {
      auto paramDefinitions = node->getChild(EnhancedAstNodeType::FunctionCallParams);
      for (unsigned i = 0; i < paramDefinitions->getChildCount(); i++) {
        auto callParam = paramDefinitions->getChild(i);
        TypeDeducer::deduceTypes(callParam->getChild(0), scope, callParam, transformWorkingData);

        if (!callParam->hasChild(EnhancedAstNodeType::TypeName)) {
          throw std::runtime_error("Cannot immediately determine type of parameter + [" + EchelonLookup::toString(i) + "]");
        }
      }
    }

    auto func = nameResolver.resolve(node, scope);

    if (func != nullptr) {
      if (func->hasChild(EnhancedAstNodeType::TypeName)) {
        typeNameResolve.setTypeName(func->getChild(EnhancedAstNodeType::TypeName)->getData());
      }
      else {
        // Missing type name.
        // TODO events.
        throw std::runtime_error("Function to call does not have a type name");
      }
    }
    else {
      // The function doesn't exist yet, mark this node as missing a dependency.
      typeNameResolve.setRefersToMissingNode(node);
    }
  }
  else {
    throw std::runtime_error("failure: type resolution requested for unhandled node type");
  }

  return typeNameResolve;
}

void TypeDeducer::mapTypeName(EnhancedAstNode *target, std::string typeNameData) {
  if (target->hasChild(EnhancedAstNodeType::TypeName)) {
    auto& data = target->getChild(EnhancedAstNodeType::TypeName)->getData();

    if (typeNameData != data) {
      throw std::runtime_error("attempt to assign type [" + typeNameData + "] to type [" + data + "]");
      // TODO type casting.
    }
  }
  else {
    EnhancedAstNode* typeName = new EnhancedAstNode();
    typeName->setNodeType(EnhancedAstNodeType::TypeName);
    typeName->setData(typeNameData);
    target->putChild(typeName);
  }
}