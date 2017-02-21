#include <echelon/transform/name-resolver.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>

EnhancedAstNode* NameResolver::resolve(EnhancedAstNode* unresolved, Scope* scope) {
  if (unresolved->getNodeType() == EnhancedAstNodeType::AccessExpression) {
    // TODO resolve from access expression.
  }
  else {
    auto nameStructure = toNameStructure(unresolved);

    auto resolved = resolveFromNameStructure(unresolved, nameStructure, scope);

    // TODO reverse iterator? might want to check the most recently added first, i.e. the closest in source code.
    // Try to find the name in any other scopes which this scope has access to.
    auto linkedScopeIterator = scope->getLinkedScopes().begin();
    while (resolved == nullptr && linkedScopeIterator != scope->getLinkedScopes().end()) {
      resolved = resolveFromNameStructure(unresolved, nameStructure, *linkedScopeIterator);
      linkedScopeIterator++;
    }

    auto parentScope = scope->getParentScope();
    while (resolved == nullptr && parentScope != nullptr) {
      resolved = resolveFromNameStructure(unresolved, nameStructure, parentScope);
      parentScope = parentScope->getParentScope();
    }

    // TODO would it make sense to check linked scopes on each parent? Just something to bear in mind.

    return resolved;
  }

  return nullptr;
}

std::queue<std::string> NameResolver::toNameStructure(EnhancedAstNode* node) {
  if (node->getNodeType() == EnhancedAstNodeType::NameStructure) {
    return toNameStructureFromNameStructureNode(node);
  }

  std::queue<std::string> nameStructure;

  auto nsItem = node;
  while (nsItem->hasChild(EnhancedAstNodeType::NameStructure)) {
    nsItem = nsItem->getChild(EnhancedAstNodeType::NameStructure);
    std::string& data = nsItem->getData();
    nameStructure.push(data);
  }
  
  std::string& data = node->getData();
  nameStructure.push(data);
  return nameStructure;
}

std::queue<std::string> NameResolver::toNameStructureFromNameStructureNode(EnhancedAstNode* node) {
  std::queue<std::string> nameStructure;

  std::string& topName = node->getData();
  nameStructure.push(topName);

  auto nsItem = node;
  while (nsItem->hasChild(EnhancedAstNodeType::NameStructure)) {
    nsItem = nsItem->getChild(EnhancedAstNodeType::NameStructure);
    std::string& data = nsItem->getData();
    nameStructure.push(data);
  }

  return nameStructure;
}

EnhancedAstNode* NameResolver::resolveFromNameStructure(EnhancedAstNode* unresolved, std::queue<std::string> nameStructure, Scope* scope) {
  auto& name = nameStructure.front();
  nameStructure.pop();

  EnhancedAstNode* found = nullptr;
  if (scope->hasModule(name)) {
    found = scope->getModule(name);
  }
  else if (scope->hasType(name)) {
    found = scope->getType(name);
  }
  else if (scope->hasFunction(name)) {
    auto functions = scope->getFunctions(name);

    // TODO temp, no matching done...
    if (functions->size() == 1) {
      found = functions->front();
      functions->pop_back();
    }

    for (auto f : *functions) {
      if (AstEnhancerHelper::doFunctionSignaturesMatch(unresolved, f)) {
        found = f;
        break;
      }
    }
  }
  else if (scope->hasPrototype(name)) {
    auto prototypes = scope->getPrototypes(name);
    // TODO really want to do a "best match" here.
    for (auto p : *prototypes) {
      if (AstEnhancerHelper::doFunctionSignaturesMatch(unresolved, p)) {
        found = p;
        break;
      }
    }
  }
  else if (scope->hasParamDefinition(name)) {
    found = scope->getParamDefinition(name);
  }
  else if (scope->hasVariable(name)) {
    found = scope->getVariable(name);
  }

  if (found != nullptr && nameStructure.size() > 0 && found->hasChild(EnhancedAstNodeType::Block)) {
    auto block = found->getChild(EnhancedAstNodeType::Block);
    return resolveFromNameStructure(unresolved, nameStructure, ((EnhancedAstBlockNode*) block)->getScope());
  }

  return found;
}

std::string NameResolver::toNameStructureString(EnhancedAstNode *node) {
  std::stringstream ss;
  auto q = toNameStructure(node);

  if (q.size()) {
    ss << q.front();
    q.pop();
  }

  while (q.size()) {
    ss << "_";
    ss << q.front();
    q.pop();
  }

  return ss.str();
}