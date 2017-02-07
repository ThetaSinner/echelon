#include <echelon/transform/name-resolver.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>

EnhancedAstNode* NameResolver::resolve(EnhancedAstNode* unresolved, Scope* scope) {
  auto nameStructure = toNameStructure(unresolved);

  return resolveInternal(unresolved, nameStructure, scope);
}

std::queue<std::string> NameResolver::toNameStructure(EnhancedAstNode* node) {
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

EnhancedAstNode* NameResolver::resolveInternal(EnhancedAstNode* unresolved, std::queue<std::string> nameStructure, Scope* scope) {
  auto& name = nameStructure.front();
  nameStructure.pop();

  EnhancedAstNode* found = nullptr;
  if (scope->hasModule(name)) {
    found = scope->getModule(name);
  }
  else if (scope->hasType(name)) {
    found = scope->getType(name);
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
    return resolveInternal(unresolved, nameStructure, ((EnhancedAstBlockNode*) block)->getScope());
  }

  return found;
}