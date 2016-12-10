#include <echelon/code-generation/generator.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <sstream>
#include <echelon/util/stream-dump.hpp>
#include <echelon/code-generation/generator-lookup.hpp>
#endif

Generator::Generator(GeneratorFunction generatorFunction) {
  this -> generatorFunction = generatorFunction;
}

std::string Generator::generate(AstNode* astNode) {
  return generatorFunction(astNode);
}
