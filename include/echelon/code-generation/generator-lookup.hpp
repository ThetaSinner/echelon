#ifndef GENERATOR_LOOKUP_HPP_INCLUDED
#define GENERATOR_LOOKUP_HPP_INCLUDED

#include <map>

#include <echelon/code-generation/generator.hpp>
#include <echelon/ast/ast-node-type-enum.hpp>

class GeneratorLookup {
  static GeneratorLookup *instance;

  std::map<AstNodeType, Generator*> generatorHash;

  GeneratorLookup() {}
  GeneratorLookup(const GeneratorLookup& gl) {}
  GeneratorLookup& operator=(const GeneratorLookup& gl) {}
public:
  static GeneratorLookup* getInstance();

  void addGenerator(AstNodeType astNodeType, Generator* generator);
  Generator* getGenerator(AstNodeType astNodeType);
};

#endif
