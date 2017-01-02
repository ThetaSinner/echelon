#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>

#include <echelon/util/stream-dump.hpp>

#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/compiler/integrity-check.hpp>

#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>
#include <echelon/transform/ast-enhancer.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/logging/logger.hpp>
#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

int main(int argc, char** args) {
  Logger* log = LoggerSharedInstance::get();

  #ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
  #else
  log->at(Level::Info) << "This is a release build.\n";
  #endif

  IntegrityCheck::StartupCheck();

  loadParserStage1Data();
  loadParserStage2Data();

  loadAstEnhancerData();

  IntegrityCheck::PostLoadCheck();

  log->at(Level::Info) << "Parser data loaded.\n";

  std::string test = "integer x = 2\nx = 3";

  Tokenizer tokenizer;
  auto tokens = tokenizer.tokenize(test);
  Parser2 parser;
  auto ast = parser.parse(tokens);

  AstEnhancer astEnhancer;
  auto enhanced = astEnhancer.enhance(ast);

  stream_dump(Level::Debug, enhanced);

  // example of overloaded functions.
  std::string func_test = "string get_string() {//gay\n} string get_string(integer t) {//af\n}";

  tokens = tokenizer.tokenize(func_test);
  ast = parser.parse(tokens);
  try {
    enhanced = astEnhancer.enhance(ast);
    stream_dump(Level::Debug, enhanced);
  }
  catch(const std::runtime_error& e) {
    std::cout << "Runtime error in enhancer: [" << e.what() << "]." << std::endl;
  }

  log->at(Level::Info) << "\nProgram will exit normally.\n";
  return 0;
}
