#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

#include <echelon/model/scope/scope-helper.hpp>
#include <echelon/model/scope/scope-pusher.hpp>
#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>
#include <echelon/model/internal/node-enhancer-lookup.hpp>
#include <echelon/model/internal/enhanced-ast-block-node.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>
#include <echelon/transform/name-resolver.hpp>
#include <echelon/transform/type-deducer.hpp>
#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>
#include <echelon/model/internal/enhanced-ast-function-node.hpp>
#include <echelon/model/internal/enhanced-ast-custom-type-node.hpp>
#include <echelon/model/internal/enhanced-ast-module-node.hpp>
#include <echelon/model/internal/enhanced-ast-variable-node.hpp>
#include <echelon/model/internal/enhanced-ast-package-node.hpp>

// TODO what was I intending to be the difference between sourceNode and nodeToMap?

void loadAstEnhancerDataInternal();

void loadAstEnhancerData() {
  static bool loaded = false;

  if (!loaded) {
    loadAstEnhancerDataInternal();
    loaded = true;
  }
}

// TODO integrity check.
void loadAstEnhancerDataInternal() {
  NameResolver nameResolver;

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Integer, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::PrimitiveValue);
    base->setNodeSubType(EnhancedAstNodeSubType::Integer);
    base->setData(input.getNodeToMap()->getData());
    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::String, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::PrimitiveValue);
    base->setNodeSubType(EnhancedAstNodeSubType::String);
    base->setData(input.getNodeToMap()->getData());
    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Decimal, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::PrimitiveValue);
    base->setNodeSubType(EnhancedAstNodeSubType::Decimal);
    base->setData(input.getNodeToMap()->getData());
    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::AccessExpression, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::AccessExpression);

    auto subInput = input;
    subInput.setTargetNode(base);
    subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::AccessStructure));

    NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::AccessStructure)(subInput);

    outputData.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::BinaryOperatorAdd, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::BinaryOperator);
    base->setNodeSubType(EnhancedAstNodeSubType::Add);

    AstEnhancerHelper::simpleMapNode(nodeToMap->getChild(0), base, input);
    AstEnhancerHelper::simpleMapNode(nodeToMap->getChild(1), base, input);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::BinaryOperatorSubtract, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::BinaryOperator);
    base->setNodeSubType(EnhancedAstNodeSubType::Subtract);

    AstEnhancerHelper::simpleMapNode(nodeToMap->getChild(0), base, input);
    AstEnhancerHelper::simpleMapNode(nodeToMap->getChild(1), base, input);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::BinaryOperatorDivide, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::BinaryOperator);
    base->setNodeSubType(EnhancedAstNodeSubType::Divide);

    AstEnhancerHelper::simpleMapNode(nodeToMap->getChild(0), base, input);
    AstEnhancerHelper::simpleMapNode(nodeToMap->getChild(1), base, input);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::ExprGroup, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::ExpressionGroup);

    AstNode* subNodeToMap = input.getNodeToMap()->getChild(0);
    AstNodeEnhancerInputData subInput = input;
    subInput.setSourceNode(input.getSourceNode());
    subInput.setTargetNode(base);
    subInput.setNodeToMap(subNodeToMap);

    NodeEnhancerLookup::getInstance()->getNodeEnhancer(subNodeToMap->getType())(subInput);

    outputData.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::TypeName, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::TypeName);
    // TODO check that the type is valid before mapping. Maybe extract type mapping functions.
    base->setData(input.getNodeToMap()->getData());

    outputData.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Expression, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::Expression);

    AstNodeEnhancerInputData subInput;
    subInput.setTargetNode(base);
    subInput.setNodeToMap(nodeToMap->getChild(0));
    
    NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getChild(0)->getType())(subInput);
    
    // Expect that expression will contain a single child which will either be an operator or a value.
    auto childToRestructure = base->getChild(0);
    base->removeChild(childToRestructure);
    auto restructured = OperatorPrecedenceTreeRestructurer::restructure(childToRestructure);
    base->putChild(restructured);

    outputData.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionParamDefinition, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionParamDefinition);
    base->setData(nodeToMap->getData());

    if (nodeToMap->hasChild(AstNodeType::TypeName)) {
      AstNode *typeNode = nodeToMap->getChild(AstNodeType::TypeName);

      AstNodeEnhancerInputData subInput = input;
      subInput.setSourceNode(nodeToMap);
      subInput.setTargetNode(base);
      subInput.setNodeToMap(typeNode);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::TypeName)(subInput);
    }

    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionParamDefinitions, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionParamDefinitions);

    for (unsigned i = 0; i < nodeToMap->getChildCount(); i++) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setSourceNode(nodeToMap);
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(i));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionParamDefinition)(subInput);
    }

    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::AccessStructure, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::AccessStructure);
    base->setData(nodeToMap->getData());

    if (nodeToMap->getChildCount() > 0) {
      auto subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(0));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getChild(0)->getType())(subInput);
    }

    outputData.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::AccessStructureFunctionCall, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::AccessStructure);
    base->setNodeSubType(EnhancedAstNodeSubType::FunctionCall);

    auto subInput = input;
    subInput.setTargetNode(base);
    subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::FunctionCall));
    NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionCall)(subInput);

    input.getTargetNode()->putChildFront(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionCall, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionCall);
    base->setData(nodeToMap->getData());

    // TODO lookup the function to call.

    if (nodeToMap->hasChild(AstNodeType::FunctionCallParams)) {
      auto subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::FunctionCallParams));
      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionCallParams)(subInput);
    }

    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionCallParams, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionCallParams);

    auto subInput = input;
    subInput.setTargetNode(base);
    for (unsigned i = 0; i < nodeToMap->getChildCount(); i++) {
      subInput.setNodeToMap(nodeToMap->getChild(i));
      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionCallParam)(subInput);
    }

    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionCallParam, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionCallParam);

    {
      auto subNodeToMap = nodeToMap->getChild(0);

      auto subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(subNodeToMap); // assume there is an expression node.
      NodeEnhancerLookup::getInstance()->getNodeEnhancer(subNodeToMap->getType())(subInput);
    }

    input.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Function, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    // Create the base node for the function.
    auto base = new EnhancedAstFunctionNode();
    base->setNodeType(EnhancedAstNodeType::Function);
    auto data = nodeToMap->getData();
    base->setData(data);

    // Map the name structure if it exists.
    bool hasNameStructure = false;
    if (nodeToMap->hasChild(AstNodeType::NameStructure)) {
      hasNameStructure = true;
      AstNodeEnhancerInputData subInput = input;
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::NameStructure));
      subInput.setTargetNode(base);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::NameStructure)(subInput);
    }

    auto scope = input.getScope();

    // Map the parameter definitions.
    Scope *paramScope = new Scope();
    if (nodeToMap->hasChild(AstNodeType::FunctionParamDefinitions)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::FunctionParamDefinitions));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionParamDefinitions)(subInput);

      auto paramDefinitions = base->getChild(EnhancedAstNodeType::FunctionParamDefinitions);
      for (auto paramDef : *paramDefinitions->getChildList()) {
        if (paramScope->hasParamDefinition(paramDef->getData())) {
          throw std::runtime_error("more than one param with the same name");
        }

        paramScope->addParamDefinition(paramDef->getData(), paramDef);
      }
    }

    if (nodeToMap->hasChild(AstNodeType::TypeName)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::TypeName));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::TypeName)(subInput);
    }

    // Map the block if there is one and handle scope logic.
    if (nodeToMap->hasChild(AstNodeType::Block)) {

      // Create a unique context name for this function.
      std::stringstream contextNameStream;
      if (hasNameStructure) {
        auto nameStructureString = nameResolver.toNameStructureString(base);
        contextNameStream << nameStructureString;

        int funcImplNum = 1;
        if (scope->hasFunctionImplementation(nameStructureString)) {
          funcImplNum += scope->getFunctionImplementations(nameStructureString)->size();
        }

        contextNameStream << "-" << funcImplNum;
      }
      else {
        contextNameStream << data;

        int funcNum = 1;
        if (scope->hasFunction(data)) {
          funcNum += scope->getFunctions(data)->size();
        }

        contextNameStream << "-" << funcNum;
      }

      // Create the new context path and set it on the base node.
      base->setContext(new Context(input.getScope()->getContext(), new ContextItem(contextNameStream.str())));

      // Creates a sub input so that the parameter definitions can be visible in the block,
      // but are not visible in the same scope as the function.
      auto subInput = input;
      auto scopeClone = new Scope(*input.getScope());
      ScopeHelper::moveTempScopeToScope(paramScope, scopeClone);
      subInput.setScope(scopeClone);

      AstEnhancerHelper::mapBlockIfPresent(nodeToMap, base, subInput);
      auto blockScope = ((EnhancedAstBlockNode*) base->getChild(EnhancedAstNodeType::Block))->getScope();

      if (hasNameStructure) {
        // add to scope as implementation
        base->setNodeSubType(EnhancedAstNodeSubType::Implementation);

        // Link the function scope to the scope of the block for the item the name structure points to.
        auto structureToLink = nameResolver.resolve(base->getChild(EnhancedAstNodeType::NameStructure), scope);
        if (structureToLink != nullptr && structureToLink->hasChild(EnhancedAstNodeType::Block)) {
          auto blockToLink = structureToLink->getChild(EnhancedAstNodeType::Block);
          ScopeHelper::linkScopes(blockScope, ((EnhancedAstBlockNode*) blockToLink)->getScope());
        }

        auto resolved = nameResolver.resolve(base, scope);
        if (resolved == nullptr) {
          throw std::runtime_error("Error [" + base->getData() + "] does not implement a prototype");
        }
        else {
          auto resolvedPrototype = (EnhancedAstFunctionNode*) resolved;
          if (resolvedPrototype->getImpl() == nullptr) {
            // Attach the implementation to its prototype.
            resolvedPrototype->setImpl(base);
          }
          else {
            throw std::runtime_error("Re-implementing function [" + base->getData() + "]");
          }
        }

        auto nameStructureString = nameResolver.toNameStructureString(base);
        if (!scope->hasFunctionImplementation(nameStructureString)) {
          scope->addFunctionImplementation(nameStructureString, base);
        }
        else {
          auto functionImplementations = scope->getFunctionImplementations(nameStructureString);
          for (auto fi : *functionImplementations) {
            if (AstEnhancerHelper::doFunctionSignaturesMatch(base, fi)) {
              throw std::runtime_error("Redeclaration of function implementation [" + data + "]");
            }
          }

          scope->addFunctionImplementation(nameStructureString, base);
        }
      }
      else {
        if (!scope->hasFunction(data)) {
          scope->addFunction(data, base);
        }
        else {
          auto functions = scope->getFunctions(data);
          for (auto f : *functions) {
            if (AstEnhancerHelper::doFunctionSignaturesMatch(base, f)) {
              throw std::runtime_error("Redeclaration of function [" + data + "]");
            }
          }

          scope->addFunction(data, base);
        }

      }

      // Get the return statement and use it to find the return type.
      TypeDeducer::deduceTypes(base->getChild(EnhancedAstNodeType::Block)->getLastChild(), blockScope, base, input.getTransformWorkingData());
      TypeEvents::triggerFunctionAdded(base, input.getTransformWorkingData());
    }
    else {
      // Prototypes don't have name structures.

      {
        // Create the context name and add it to the base node.
        std::stringstream contextNameStream;
        contextNameStream << data;

        int prototypeNum = 1;
        if (scope->hasParamDefinition(data)) {
          prototypeNum += scope->getPrototypes(data)->size();
        }

        contextNameStream << "-" << prototypeNum;

        // Create the new context path and set it on the base node.
        base->setContext(new Context(input.getScope()->getContext(), new ContextItem(contextNameStream.str())));
      }

      if (!scope->hasPrototype(data)) {
        scope->addPrototype(data, base);
      }
      else {
        auto prototypes = scope->getPrototypes(data);
        for (auto p : *prototypes) {
          if (AstEnhancerHelper::doFunctionSignaturesMatch(base, p)) {
            throw std::runtime_error("Redeclaration of prototype [" + data + "]");
          }
        }

        scope->addPrototype(data, base);
      }

      base->setNodeSubType(EnhancedAstNodeSubType::Prototype);
    }

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Package, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstPackageNode();
    base->setNodeType(EnhancedAstNodeType::Package);
    base->setData(nodeToMap->getData());
    base->setContext(new Context(input.getScope()->getContext(), new ContextItem(base->getData())));
    outputData.getTargetNode()->putChild(base);

    // directly update on the current scope.
    // Here's another reason the package declaration has to be the first thing in any scope.
    // Otherwise everything already on the scope would have to be manually updated.
    input.getScope()->setContext(new Context(input.getScope()->getContext(), new ContextItem(base->getData())));

    if (nodeToMap->getChildCount() > 0) {
      AstNodeEnhancerInputData subInput(input);
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(0));

      auto& packageEnhancer = NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Package);
      auto subOutput = packageEnhancer(subInput);

      return subOutput;
    }

    outputData.setTargetNode(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Variable, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto data = nodeToMap->getData();
    auto base = new EnhancedAstVariableNode();
    base->setNodeType(EnhancedAstNodeType::Variable);
    base->setData(data);

    // Add a context path to the variable.
    base->setContext(new Context(input.getScope()->getContext(), new ContextItem(data)));

    if (nodeToMap->hasChild(AstNodeType::TypeName)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::TypeName));
      subInput.setTargetNode(base);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::TypeName)(subInput);
    }

    // This is the value to assign.
    if (nodeToMap->hasChild(AstNodeType::Expression)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::Expression));
      subInput.setTargetNode(base);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Expression)(subInput);
    }

    // This is the first time we've seen this variable in this scope, add it.
    auto scope = input.getScope();
    if (!scope->hasVariable(data)) {
      base->setNodeSubType(EnhancedAstNodeSubType::Declaration);
      scope->addVariable(data, base);

      // Determine the type from the value assigned.
      if (base->hasChild(EnhancedAstNodeType::Expression)) {
        auto expression = base->getChild(EnhancedAstNodeType::Expression)->getChild(0);
        TypeDeducer::deduceTypes(expression, input.getScope(), base, input.getTransformWorkingData());
      }
    }
    else {
      // The variable has been seen before. Check that there is no type declaration.
      if (nodeToMap->hasChild(AstNodeType::TypeName)) {
        std::string message = "Error, redeclaration of variable [" + data + "].";
        throw std::runtime_error(message.c_str());
      }

      base->setNodeSubType(EnhancedAstNodeSubType::Assign);
    }

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Module, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstModuleNode();
    base->setNodeType(EnhancedAstNodeType::Module);
    base->setData(nodeToMap->getData());

    // Add module to scope.
    ScopePusher::push(input.getScope(), base);

    base->setContext(new Context(input.getScope()->getContext(), new ContextItem(base->getData())));

    // Map the module block.
    AstEnhancerHelper::mapBlockIfPresent(nodeToMap, base, input);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Block, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstBlockNode();
    base->setNodeType(EnhancedAstNodeType::Block);

    Scope* blockScope = ScopeHelper::createChildScope(input.getScope());
    base->setScope(blockScope);

    if (input.getUpdatedContext() != nullptr) {
      // Map the updated context which includes the the path to the current block owner.
      blockScope->setContext(input.getUpdatedContext());
      // Explicitly wipe the updated context. The input is default copy constructed and passed down
      // so don't risk it being used again incorrectly.
      input.setUpdatedContext(nullptr);
    }
    else {
      // TODO this can throw once contexts are implemented to prevent future breakages
      // throw std::runtime_error("Compiler failure: no updated context to map to block scope");
    }

    AstNodeEnhancerInputData subInput = input;
    subInput.setTargetNode(base);
    subInput.setScope(blockScope);
    for (unsigned i = 0; i < nodeToMap->getChildCount(); i++) {
      subInput.setNodeToMap(nodeToMap->getChild(i));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getChild(i)->getType())(subInput);
    }

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::SingleLineComment, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::SingleLineComment);
    base->setData(nodeToMap->getData());

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Type, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstCustomTypeNode();
    base->setNodeType(EnhancedAstNodeType::CustomType);
    base->setData(nodeToMap->getData());

    base->setContext(new Context(input.getScope()->getContext(), new ContextItem(base->getData())));

    input.getScope()->addType(nodeToMap->getData(), base);

    AstEnhancerHelper::mapBlockIfPresent(nodeToMap, base, input);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::BinaryOperatorMultiply, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::BinaryOperator);
    base->setNodeSubType(EnhancedAstNodeSubType::Multiply);

    AstNodeEnhancerInputData subInput = input;
    subInput.setNodeToMap(nodeToMap->getChild(0));
    subInput.setTargetNode(base);
    NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getChild(0)->getType())(subInput);

    subInput.setNodeToMap(nodeToMap->getChild(1));
    NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getChild(1)->getType())(subInput);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::VariableValue, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::VariableValue);
    base->setData(nodeToMap->getData());

    AstEnhancerHelper::mapChildIfPresent(base, nodeToMap, input, AstNodeType::NameStructure);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::NameStructure, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::NameStructure);
    base->setData(nodeToMap->getData());

    if (nodeToMap->hasChild(AstNodeType::NameStructure)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::NameStructure));
      subInput.setTargetNode(base);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::NameStructure)(subInput);
    }

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });
}