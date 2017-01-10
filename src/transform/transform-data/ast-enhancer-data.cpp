#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

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
  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Integer,
                                                     [](AstNode *node, Scope scope) -> EnhancedAstNode * {
                                                       auto base = new EnhancedAstNode();

                                                       base->setNodeType(EnhancedAstNodeType::PrimitiveValue);
                                                       base->setNodeSubType(EnhancedAstNodeSubType::Integer);
                                                       base->setData(node->getData());

                                                       return base;
                                                     });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::TypeName,
                                                     [](AstNode *node, Scope scope) -> EnhancedAstNode * {
                                                       auto base = new EnhancedAstNode();
                                                       base->setNodeType(EnhancedAstNodeType::TypeName);
                                                       // TODO check that the type is valid before mapping. Maybe extract type mapping functions.
                                                       base->setData(node->getData());

                                                       return base;
                                                     });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionParamDefinition,
                                                     [](AstNode *node, Scope scope) -> EnhancedAstNode * {
                                                       auto base = new EnhancedAstNode();
                                                       base->setNodeType(EnhancedAstNodeType::FunctionParamDefinition);
                                                       base->setData(node->getData());

                                                       if (node->hasChild(AstNodeType::Type)) {
                                                         AstNode *typeNode = node->getChild(AstNodeType::Type);
                                                         base->putChild(
                                                             NodeEnhancerLookup::getInstance()->getNodeEnhancer(
                                                                 typeNode->getType())(typeNode, scope));
                                                       }

                                                       return base;
                                                     });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::FunctionParamDefinitions,
                                                     [](AstNode *node, Scope scope) -> EnhancedAstNode * {
                                                       auto base = new EnhancedAstNode();
                                                       base->setNodeType(EnhancedAstNodeType::FunctionParamDefinitions);

                                                       for (unsigned i = 0; i < node->getChildCount(); i++) {
                                                         base->putChild(
                                                             NodeEnhancerLookup::getInstance()->getNodeEnhancer(
                                                                 node->getChild(i)->getType())(node->getChild(i),
                                                                                               scope));
                                                       }

                                                       return base;
                                                     });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Function,
                                                     [](AstNode *node, Scope scope) -> EnhancedAstNode * {
                                                       auto base = new EnhancedAstNode();
                                                       base->setNodeType(EnhancedAstNodeType::Function);
                                                       base->setData(node->getData());

                                                       // TODO map return type.

                                                       if (node->hasChild(AstNodeType::FunctionParamDefinitions)) {
                                                         AstNode *paramDefinitionsNode = node->getChild(
                                                             AstNodeType::FunctionParamDefinitions);
                                                         auto enhancedParamDefinitions = NodeEnhancerLookup::getInstance()->getNodeEnhancer(
                                                             paramDefinitionsNode->getType())(paramDefinitionsNode,
                                                                                              scope);
                                                         if (enhancedParamDefinitions != nullptr) {
                                                           base->putChild(enhancedParamDefinitions);
                                                         }
                                                       }

                                                       // TODO map block.

                                                       return base;
                                                     });

  NodeEnhancerLookup::getInstance()->addNodeEnhancer(AstNodeType::Package, [](AstNode *node, Scope scope) -> EnhancedAstNode * {
    auto base = new EnhancedAstNode();
    base->setNodeType(EnhancedAstNodeType::Package);
    base->setData(node->getData());

    if (node->getChildCount() > 0) {
      auto& packageEnhancer = NodeEnhancerLookup::getInstance()->getNodeEnhancer(AstNodeType::Package);
      base->putChild(packageEnhancer(node->getChild(0), scope));
    }

    return base;
  });
}