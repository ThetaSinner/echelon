#ifndef ECHELON_CODE_GENERATOR_HPP_INCLUDED
#define ECHELON_CODE_GENERATOR_HPP_INCLUDED

#include <echelon/code-generation/code-generator.hpp>

class EchelonCodeGenerator : public CodeGenerator {
public:
  std::string generate(AstNode *root);
};

#endif
