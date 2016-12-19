#include <echelon/compiler/echelon-compiler.hpp>

std::string EchelonCompiler::compile(std::string input) {
  auto tokens = tokenizer.tokenize(input);
  auto ast = parser.parse(tokens);

  typeDeductionEngine.deduceTypes(ast);

  return codeGenerator.generate(ast);
}