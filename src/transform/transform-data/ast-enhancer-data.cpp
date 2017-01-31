#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>
#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>
#include <echelon/transform/transform-data/ast-enhancer-helper.hpp>
#include <echelon/transform/type-deducer.hpp>

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

    outputData.getTargetNode()->putChild(base);

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionParamDefinition, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionParamDefinition);
    base->setData(input.getSourceNode()->getData());

    if (input.getSourceNode()->hasChild(AstNodeType::Type)) {
      AstNode *typeNode = input.getSourceNode()->getChild(AstNodeType::Type);

      AstNodeEnhancerInputData subInput;
      subInput.setSourceNode(input.getSourceNode());
      subInput.setTargetNode(base);
      subInput.setNodeToMap(typeNode);
      subInput.setScope(input.getScope());

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Type)(subInput);
    }

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionParamDefinitions, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
    AstNodeEnhancerOutputData outputData(input);

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::FunctionParamDefinitions);

    for (unsigned i = 0; i < input.getSourceNode()->getChildCount(); i++) {
      AstNodeEnhancerInputData subInput;
      subInput.setSourceNode(input.getSourceNode());
      subInput.setTargetNode(base);
      subInput.setNodeToMap(input.getSourceNode()->getChild(i));
      subInput.setScope(input.getScope());

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionParamDefinition)(subInput);
    }

    return outputData;
  });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Function, [](AstNodeEnhancerInputData input) -> AstNodeEnhancerOutputData {
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

    // Map the parameter definitions.
    if (nodeToMap->hasChild(AstNodeType::FunctionParamDefinitions)) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setTargetNode(base);
      subInput.setNodeToMap(nodeToMap->getChild(AstNodeType::FunctionParamDefinitions));

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionParamDefinitions)(subInput);
    }

    // TODO these params need to be mapped onto the scope.

    // Map the block if there is one and handle scope logic.
    auto scope = input.getScope();
    if (nodeToMap->hasChild(AstNodeType::Block)) {
      AstEnhancerHelper::mapBlockIfPresent(nodeToMap, base, input);

      TypeDeducer::deduceTypes(base->getChild(EnhancedAstNodeType::Block)->getLastChild(), scope, base);

      if (hasNameStructure) {
        // add to scope as implementation
        base->setNodeSubType(EnhancedAstNodeSubType::Implementation);

        // It is important the name structure, function name and params have been mapped.
        auto functionHash = AstEnhancerHelper::computeFunctionHash(base);
        if (!scope->hasFunctionImplementation(functionHash)) {
          scope->addFunctionImplementation(functionHash, base);
        }
        else {
          throw std::runtime_error("Re-implementing function [" + functionHash + "]");
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
    }
    else {
      // Prototypes don't have name structures.

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