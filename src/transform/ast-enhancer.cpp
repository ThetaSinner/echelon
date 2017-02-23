#include <echelon/transform/ast-enhancer.hpp>

#include <functional>

#include <echelon/model/scope/scope.hpp>
#include <echelon/transform/ast-node-enhancer-input-data.hpp>
#include <echelon/transform/ast-node-enhancer-output-data.hpp>
#include <echelon/transform/transform-working-data.hpp>
#include <echelon/model/internal/node-enhancer-lookup.hpp>
#include <echelon/model/internal/enhanced-ast-block-node.hpp>

void enhanceInternal(AstNode *node, EnhancedAstNode *target, Scope* scope, TransformWorkingData* transformWorkingData) {
  auto working_target = target;

  for (unsigned i = 0; i < node->getChildCount(); i++) {
    NodeEnhancer& nodeEnhancer = NodeEnhancerLookup::getInstance()->getNodeEnhancer(node->getChild(i)->getType());

    AstNodeEnhancerInputData input;
    input.setSourceNode(node);
    input.setNodeToMap(node->getChild(i));
    input.setTargetNode(working_target);
    input.setScope(scope);
    input.setTransformWorkingData(transformWorkingData);

    auto output = nodeEnhancer(input);

    // jump to the new working nodes. This allows the enhancer to modify the ast structure.
    working_target = output.getTargetNode();
  }
}

EnhancedAstNode *AstEnhancer::enhance(AstNode *node) {
  Scope* scope = new Scope();
  scope->setContext(new Context(nullptr, new ContextItem("context-root")));

  TransformWorkingData *transformWorkingData = new TransformWorkingData();

  auto root = new EnhancedAstBlockNode(); // TODO is this okay?
  root->setNodeType(EnhancedAstNodeType::Program);
  root->setData(node->getData());
  root->setScope(scope);

  enhanceInternal(node, root, scope, transformWorkingData);

  if (transformWorkingData->getEventContainer().hasListeners()) {
    // At some point this will be allowed, provided the resulting program is going to the linker.
    // This check can then go in the compiler? check if the programs integrity is okay.

    //throw std::runtime_error("There are unresolved references, program is invalid.");
  }

  return root;
}

