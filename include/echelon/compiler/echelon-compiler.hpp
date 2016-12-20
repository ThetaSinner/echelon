#ifndef ECHELON_COMPILER_HPP_INCLUDED
#define ECHELON_COMPILER_HPP_INCLUDED

#include <string>

#include <echelon/parser/stage2/parser.hpp>
#include <echelon/code-generation/code-generator.hpp>
#include <echelon/transform/type-deduction-engine.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>

class EchelonCompiler {
  Tokenizer tokenizer;
  Parser2 parser;
  TypeDeductionEngine typeDeductionEngine;
  CodeGenerator* codeGenerator;
public:
    EchelonCompiler();

    std::string compile(std::string input);
};

#endif
