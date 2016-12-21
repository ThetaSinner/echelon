#include <echelon/compiler/echelon-compiler.hpp>

#include <echelon/code-generation/code-generator-factory.hpp>

#ifdef ECHELON_DEBUG
#include <fstream>
#include <echelon/util/ast-to-graphviz.hpp>
#endif

EchelonCompiler::EchelonCompiler() {
  codeGenerator = CodeGeneratorFactory::newCodeGenerator();
}

void EchelonCompiler::setCodeGenerator(CodeGenerator* codeGenerator) {
  this -> codeGenerator = codeGenerator;
}

std::string EchelonCompiler::compile(std::string input) {
  auto tokens = tokenizer.tokenize(input);
  auto ast = parser.parse(tokens);
  #ifdef ECHELON_DEBUG
  std::ofstream f("compiler-ast-out.gv", std::ios::out);
  f << toGraphviz(ast);
  f.close();
  #endif

  typeDeductionEngine.deduceTypes(ast);

  return codeGenerator -> generate(ast);
}