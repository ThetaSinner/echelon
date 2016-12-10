#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include <functional>

#include <echelon/ast/AstNode.hpp>

typedef std::function<void(AstNode* node)> GeneratorFunction;

class Generator {
private:
  GeneratorFunction generatorFunction;
public:
  void setGenerator(GeneratorFunction generatorFunction);
  GeneratorFunction getGenerator();
};

#endif
