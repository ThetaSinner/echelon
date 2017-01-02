#include <echelon/compiler/echelon-compiler.hpp>

#include <echelon/code-generation/code-generator-factory.hpp>
#include <echelon/util/ast-to-graphviz.hpp>
#include <echelon/compiler/integrity-check.hpp>
#include <echelon/transform/transform-data/ast-enhancer-data.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>

#ifdef ECHELON_DEBUG
#include <fstream>
#include <echelon/util/logging/logger-shared-instance.hpp>
#endif

EchelonCompiler::EchelonCompiler() {
  try {
    IntegrityCheck::StartupCheck();

    loadParserStage1Data();
    loadParserStage2Data();

    loadAstEnhancerData();

    IntegrityCheck::PostLoadCheck();

    codeGenerator = CodeGeneratorFactory::newCodeGenerator();
  }
  catch (const std::runtime_error& e) {
    LoggerSharedInstance::get()->at(Level::Fatal) << e.what();
    throw e;
  }
}

// TODO test scope only.
void EchelonCompiler::setCodeGenerator(CodeGenerator* codeGenerator) {
  this -> codeGenerator = codeGenerator;
}

std::list<Token*> EchelonCompiler::tokenize(std::string input) {
  return tokenizer.tokenize(input);
}

AstNode* EchelonCompiler::parse(std::list<Token*> input) {
  return parser.parse(input);
}

AstNode* EchelonCompiler::parse(std::string input) {
  return parse(tokenize(input));
}

EnhancedAstNode* EchelonCompiler::enhance(AstNode* input) {
  return astEnhancer.enhance(input);
}

EnhancedAstNode* EchelonCompiler::enhance(std::string input) {
  return enhance(parse(tokenize(input)));
}

std::string EchelonCompiler::compile(std::string input) {
  try {
    auto tokens = tokenize(input);
    auto ast = parse(tokens);
    #ifdef ECHELON_DEBUG
    std::ofstream f("compiler-ast-out.gv", std::ios::out);
    f << toGraphviz(ast);
    f.close();
    #endif

    typeDeductionEngine.deduceTypes(ast);

    return codeGenerator -> generate(ast);
  }
  catch (const std::runtime_error& e) {
    #ifdef ECHELON_DEBUG
    LoggerSharedInstance::get()->at(Level::Fatal) << "Compilation failed: [" << e.what() << "]\n";
    #endif
    throw std::runtime_error("Compilation failed.");
  }
  catch (...) {
    throw std::runtime_error("Compilation failed with unknown reason.");
  }
}