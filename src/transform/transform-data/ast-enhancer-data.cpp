#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>
#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>

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
    base->setNodeType(EnhancedAstNodeType::PrimitiveValue); // TODO this isn't how I want to handle types...
    base->setNodeSubType(EnhancedAstNodeSubType::Integer);
    base->setData(input.getNodeToMap()->getData());
    input.getTargetNode()->putChild(base);

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

    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::Function);
    base->setData(input.getSourceNode()->getData());

    // TODO map return type.

    if (input.getSourceNode()->hasChild(AstNodeType::FunctionParamDefinitions)) {
      AstNode *paramDefinitionsNode = input.getSourceNode()->getChild(AstNodeType::FunctionParamDefinitions);

      AstNodeEnhancerInputData subInput;
      subInput.setSourceNode(input.getSourceNode());
      subInput.setTargetNode(base);
      subInput.setNodeToMap(paramDefinitionsNode);
      subInput.setScope(input.getScope());

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::FunctionParamDefinitions)(subInput);
    }

    // TODO map block.

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

    // This is the first time we've seen this variable in this scope, add it.
    auto scope = input.getScope();
    if (!scope->hasVariable(data)) {
      base->setNodeSubType(EnhancedAstNodeSubType::Declaration);
      scope->addVariable(data, base);
    }
    else {
      // The variable has been seen before. Check that there is no type declaration.
      if (nodeToMap->hasChild(AstNodeType::TypeName)) {
        std::string message = "Error, redeclaration of variable [" + data + "].";
        throw std::runtime_error(message.c_str());
      }

      base->setNodeSubType(EnhancedAstNodeSubType::Assign);
    }

    // TODO this means a variable declaration can refer to itself because it will be on the scope..
    // expect to map type node and expression to assign if present.
    for (unsigned i = 0; i < nodeToMap->getChildCount(); i++) {
      AstNodeEnhancerInputData subInput = input;
      subInput.setNodeToMap(nodeToMap->getChild(i));
      subInput.setTargetNode(base);

      NodeEnhancerLookup::getInstance()->getNodeEnhancer(nodeToMap->getChild(i)->getType())(subInput);
    }

    outputData.getTargetNode()->putChild(base);
    return outputData;
  });
}