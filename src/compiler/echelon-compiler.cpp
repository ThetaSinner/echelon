#include <echelon/compiler/echelon-compiler.hpp>

#include <echelon/parser/stage1/tokenizer.hpp>


std::string EchelonCompiler::compile(std::string input) {
  auto tokens = tokenize(input);
  auto ast = parser.parse(tokens);

  typeDeductionEngine.deduceTypes(ast);

  return codeGenerator.generate(ast);
}