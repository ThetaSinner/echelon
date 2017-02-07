#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>
#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>
#include <echelon/transform/name-resolver.hpp>
#include <echelon/transform/type-deducer.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-function-prototype-node.hpp>
#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>

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

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Decimal, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::PrimitiveValue);
    base->setNodeSubType(EnhancedAstNodeSubType::Decimal);
    base->setData(input.getNodeToMap()->getData());
    input.getTargetNode()->putChild(base);

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

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Function, [&nameResolver](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    // Create the base node for the function.
    auto base = new EnhancedAstNode();
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
    if (nodeToMap->hasChild(AstNodeType::FunctionParamDefinitions)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::FunctionParamDefinitions));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionParamDefinitions)(subInput);

      auto paramDefinitions = base->getChild(EnhancedAstNodeType::FunctionParamDefinitions);
      for (auto paramDef : *paramDefinitions->getChildList()) {
        if (scope->hasParamDefinition(paramDef->getData())) {
          throw std::runtime_error("more than one param with the same name");
        }

        scope->addParamDefinition(paramDef->getData(), paramDef);
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
      AstEnhancerHelper::mapBlockIfPresent(nodeToMap, base, input);
      auto blockScope = ((EnhancedAstBlockNode*) base->getChild(EnhancedAstNodeType::Block))->getScope();

      if (hasNameStructure) {
        // add to scope as implementation
        base->setNodeSubType(EnhancedAstNodeSubType::Implementation);

        // Link the function scope to the scope of the block for the item the name structure points to.
        auto nameStructureResolved = nameResolver.resolve(base->getChild(EnhancedAstNodeType::NameStructure), scope);
        if (nameStructureResolved != nullptr && nameStructureResolved->hasChild(EnhancedAstNodeType::Block)) {
          auto block = nameStructureResolved->getChild(EnhancedAstNodeType::Block);
          blockScope->pushLinkedScope(((EnhancedAstBlockNode*) block)->getScope());
          // TODO it's now really important that scopes have "local and parent" as suggested in some other TODO, otherwise we're importing a whole bunch of things we shouldn't be.
        }

        auto resolved = nameResolver.resolve(base, scope);
        if (resolved == nullptr) {
          throw std::runtime_error("Error [" + base->getData() + "] does not implement a prototype");
        }
        else {
          auto resolvedPrototype = (EnhancedAstFunctionPrototypeNode*) resolved;
          if (resolvedPrototype->getImpl() == nullptr) {
            // Attach the implementation to its prototype.
            resolvedPrototype->setImpl(base);
          }
          else {
            throw std::runtime_error("Re-implementing function [" + base->getData() + "]");
          }
        }
      }
      else {
        if (!scope->hasFunction(data)) {
          scope->addFunction(data, base);
        }
        else {
          auto& functions = scope->getFunctions(data);
          for (auto f : functions) {
            if (AstEnhancerHelper::doFunctionSignaturesMatch(base, f)) {
              throw std::runtime_error("Redeclaration of function [" + data + "]");
            }
          }

          scope->addFunction(data, base);
        }
      }

      // Get the return statement and use it to find the return type.
      TypeDeducer::deduceTypes(base->getChild(EnhancedAstNodeType::Block)->getLastChild(), blockScope, base);
    }
    else {
      // Prototypes don't have name structures.

      // The prototype is a special node, construct it now.
      base = new EnhancedAstFunctionPrototypeNode(base);

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

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::Package);
    base->setData(nodeToMap->getData());
    outputData.getTargetNode()->putChild(base);

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
    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::Variable);
    base->setData(data);

    if (nodeToMap->hasChild(AstNodeType::TypeName)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::TypeName));
      subInput.setTargetNode(base);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::TypeName)(subInput);
    }

    // This is the value to assign. Not a very nice way to access it.
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
        TypeDeducer::deduceTypes(expression, input.getScope(), base);
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

    auto data = nodeToMap->getData();
    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::Module);
    base->setData(data);

    if (!input.getScope()->hasModule(data)) {
      input.getScope()->addModule(data, base);
    }
    else {
      throw std::runtime_error("Error, module " + data + "] already exists.");
    }

    AstEnhancerHelper::mapBlockIfPresent(nodeToMap, base, input);

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Block, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto nodeToMap = input.getNodeToMap();

    auto base = new EnhancedAstBlockNode();
    base->setNodeType(EnhancedAstNodeType::Block);

    Scope* blockScope = new Scope(*input.getScope());
    base->setScope(blockScope);

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

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::CustomType);
    base->setData(nodeToMap->getData());

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