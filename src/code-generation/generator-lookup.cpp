#include <echelon/code-generation/generator-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <stdexcept>
#include <echelon/parser/stage2/echelon-lookup.hpp>
#endif

GeneratorLookup *GeneratorLookup::instance = nullptr;

GeneratorLookup* GeneratorLookup::getInstance() {
  if (instance == nullptr) {
    instance = new GeneratorLookup();
  }

  return instance;
}

void GeneratorLookup::addGenerator(AstNodeType astNodeType, Generator* generator) {
  generatorHash.insert({astNodeType, generator});
}

Generator* GeneratorLookup::getGenerator(AstNodeType astNodeType) {
  #ifdef ECHELON_DEBUG
  if (generatorHash.find(astNodeType) == generatorHash.end()) {
    std::cout << "Missing generator [" << EchelonLookup::toString(astNodeType) << "]" << std::endl;
    throw std::runtime_error("Missing generator");
  }
  #endif

  return generatorHash.at(astNodeType);
}
