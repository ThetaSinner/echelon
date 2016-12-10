#ifndef GENERATOR_HPP_INCLUDED
#define GENERATOR_HPP_INCLUDED

#include <functional>
#include <string>

#include <echelon/ast/AstNode.hpp>

typedef std::function<std::string(AstNode*)> GeneratorFunction;

class Generator {
private:
   GeneratorFunction generatorFunction;
public:
  Generator(GeneratorFunction generatorFunction);
  std::string generate(AstNode* astNode);
};

#endif
