#ifndef CODE_GENERATOR_HPP_INCLUDED
#define CODE_GENERATOR_HPP_INCLUDED

#include <string>

#include <echelon/model/ast/ast-node.hpp>

class CodeGenerator {
public:
  virtual std::string generate(AstNode *root) = 0;
};

#endif
