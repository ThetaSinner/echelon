#ifndef ECHELON_COMPILER_HPP_INCLUDED
#define ECHELON_COMPILER_HPP_INCLUDED

#include <string>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/code-generation/code-generator.hpp>
#include <echelon/transform/type-deduction-engine.hpp>

class EchelonCompiler {
  Parser2 parser;
  CodeGenerator codeGenerator;
  TypeDeductionEngine typeDeductionEngine;
public:
    std::string compile(std::string input);
};

#endif
