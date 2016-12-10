#include <echelon/code-generation/generator.hpp>

void Generator::setGenerator(GeneratorFunction generatorFunction) {
  this -> generatorFunction = generatorFunction;
}

GeneratorFunction Generator::getGenerator() {
  return generatorFunction;
}
