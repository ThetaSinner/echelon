#include <echelon/model/scope/scope-pusher.hpp>

void ScopePusher::push(Scope* scope, EnhancedAstNode* node) {
  // TODO implement event triggering.
  auto& name = node->getData();
  auto type = node->getNodeType();

  switch (type) {
    case EnhancedAstNodeType::Module:
      if (scope->hasModule(name)) {
        throw std::runtime_error("The compiler should have joined the two module definitions...");
      }

      scope->addModule(name, node);
      break;
    case EnhancedAstNodeType::CustomType:
      if (scope->hasType(name)) {
        throw std::runtime_error("Redeclaring type: " + name);
      }

      scope->addType(name, node);

      // trigger event.
      break;
    case EnhancedAstNodeType::Function:
      if (node->getNodeSubType() == EnhancedAstNodeSubType::Prototype) {
        if (scope->hasPrototype(name)) {
          auto prototypes = scope->getPrototypes(name);

          for (auto p : *prototypes) {
            if (AstEnhancerHelper::doFunctionSignaturesMatch(node, p)) {
              throw std::runtime_error("Conflicting prototype declaration: " + name);
            }
          }
        }

        scope->addParamDefinition(name, node);
      }
      else {
        if (scope->hasFunction(name)) {
          auto functions = scope->getFunctions(name);

          for (auto f : *functions) {
            if (AstEnhancerHelper::doFunctionSignaturesMatch(node, f)) {
              throw std::runtime_error("Conflicting function declaration: " + name);
            }
          }
        }

        scope->addFunction(name, node);
      }

      break;
    case EnhancedAstNodeType::Variable:
      if (scope->hasVariable(name) || scope->hasParamDefinition(name)) {
        throw std::runtime_error("Conflicting variable declaration: " + name);
      }

      scope->addVariable(name, node);
      break;
    case EnhancedAstNodeType::FunctionParamDefinition:
      if (scope->hasParamDefinition(name)) {
        // TODO this shouldn't happen really, the params aren't allowed to have the same name and this should be checked before being pushed to the scope, so that errors can be constructed for the function.
        throw std::runtime_error("Conflicting param definition: " + name);
      }

      scope->addParamDefinition(name, node);
      break;
    default:
      throw std::runtime_error("Attempt to push node to scope which is not handled.");
  }
}