#ifndef CODE_GENERATOR_HPP_INCLUDED
#define CODE_GENERATOR_HPP_INCLUDED

#include <string>
#include <echelon/ast/AstNode.hpp>

class CodeGenerator {
public:
  std::string generate(AstNode* root);
};

#endif
