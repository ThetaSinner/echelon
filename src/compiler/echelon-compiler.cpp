#include <echelon/compiler/echelon-compiler.hpp>

#include <echelon/code-generation/code-generator-factory.hpp>

EchelonCompiler::EchelonCompiler() {
  codeGenerator = CodeGeneratorFactory::newCodeGenerator();
}

void EchelonCompiler::setCodeGenerator(CodeGenerator* codeGenerator) {
  this -> codeGenerator = codeGenerator;
}

std::string EchelonCompiler::compile(std::string input) {
  auto tokens = tokenizer.tokenize(input);
  auto ast = parser.parse(tokens);

  typeDeductionEngine.deduceTypes(ast);

  return codeGenerator -> generate(ast);
}