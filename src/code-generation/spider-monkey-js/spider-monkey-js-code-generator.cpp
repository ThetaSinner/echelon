#include "spider-monkey-js-code-generator.hpp"

#include "spider-monkey-js-generators.hpp"

#include <echelon/code-generation/generator-lookup.hpp>

std::string SpiderMonkeyJSCodeGenerator::generate(AstNode* root) {
  loadGenerators();

  return GeneratorLookup::getInstance() -> getGenerator(root -> getType()) -> generate(root);
}
