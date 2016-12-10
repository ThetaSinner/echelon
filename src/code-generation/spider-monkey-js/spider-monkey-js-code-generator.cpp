#include <echelon/code-generation/code-generator.hpp>

#include "spider-monkey-js-generators.hpp"

#include <echelon/code-generation/generator-lookup.hpp>

std::string CodeGenerator::generate(AstNode* root) {
  loadGenerators();

  return GeneratorLookup::getInstance() -> getGenerator(root -> getType()) -> generate(root);
}
