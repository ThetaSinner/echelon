#ifndef CODE_GENERATOR_HPP_INCLUDED
#define CODE_GENERATOR_HPP_INCLUDED

#include <string>

#include <echelon/ast/ast-node.hpp>

class CodeGenerator {
public:
  std::string generate(AstNode* root);
};

#endif
