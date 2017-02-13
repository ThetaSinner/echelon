#include <echelon/transform/custom-type-handler.hpp>

void CustomTypeHandler::handle(EnhancedAstNode* customType) {
  static EchelonCompiler compiler; // TODO get current compiler instance.
  if (findFunction(customType, "create") == nullptr) { // TODO hard coding method names...
    std::stringstream ss;
    ss << "type " << customType->getData() << " { ";
    ss << "function create() { // compiler generated stub.\n}";
    ss << "}";

    auto enhanced = compiler.enhance(ss.str());

    // TODO this is actually a linker step.. can't just add because I have to worry about scope.
    // TODO ((EnhancedAstBlockNode*) customType->getChild(EnhancedAstNodeType::Block))->put
  }
}

EnhancedAstNode* CustomTypeHandler::findFunction(EnhancedAstNode* customType, std::string name) {
  if (customType->hasChild(EnhancedAstNodeType::Block)) {
    auto scope = ((EnhancedAstBlockNode*) customType->getChild(EnhancedAstNodeType::Block)) ->getScope();

    auto prototypes = scope->getPrototypes(name);
    if (prototypes->size() == 1) {
      return prototypes->front();
    }
    else if (prototypes->size() > 1) {
      throw std::runtime_error("Prototype not unique, this case is not handled yet.");
    }

    auto functions = scope->getFunctions(name);
    if (functions->size() == 1) {
      return functions->front();
    }
    else if (functions->size() > 1) {
      throw std::runtime_error("Function not unique, this case is not handled yet.");
    }
  }

  return nullptr;
}