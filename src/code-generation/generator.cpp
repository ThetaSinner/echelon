#include <echelon/code-generation/generator.hpp>

Generator::Generator(GeneratorFunction generatorFunction) {
  this -> generatorFunction = generatorFunction;
}

std::string Generator::generate(AstNode* astNode) {
  return generatorFunction(astNode);
}
