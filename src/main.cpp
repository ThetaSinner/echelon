#include <iostream>

#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>

#include <echelon/util/stream-dump.hpp>

#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/compiler/integrity-check.hpp>

#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>
#include <echelon/transform/ast-enhancer.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

int main(int argc, char** args) {
  #ifdef ECHELON_DEBUG
  std::cout << "This is a debug build.\n";
  #else
  std::cout << "This is a release build.\n";
  #endif

  loadParserStage1Data();
  loadParserStage2Data();

  IntegrityCheck::StartupCheck();

  std::cout << "Parser data loaded." << std::endl;

  // Need a good data structure which has all the declared data, types and functions.
  // It needs to be such that it is simple to find out whether something is defined in the current context or parent context
  // for example. Maybe it will be a complementary structure to the existing tree.

  NodeEnhancerLookup::getInstance() -> addNodeEnhancer(AstNodeType::Type, [] (AstNode* node, Scope scope) -> EnhancedAstNode* {
    auto base = new EnhancedAstNode();

    base -> setNodeType(EnhancedAstNodeType::Type);

    // TODO type mapping
    //base -> setNodeSubType(EnhancedAstNodeSubType::IntegerType);
    base -> setData(node -> getData());

    return base;
  });

  NodeEnhancerLookup::getInstance() -> addNodeEnhancer(AstNodeType::Integer, [] (AstNode* node, Scope scope) -> EnhancedAstNode* {
    auto base = new EnhancedAstNode();

    base -> setNodeType(EnhancedAstNodeType::PrimitiveValue);
    base -> setNodeSubType(EnhancedAstNodeSubType::Integer);
    base -> setData(node -> getData());

    return base;
  });

  std::string test = "integer x = 2\nx = 3";

  Tokenizer tokenizer;
  auto tokens = tokenizer.tokenize(test);
  Parser2 parser;
  auto ast = parser.parse(tokens);

  AstEnhancer astEnhancer;
  auto enhanced = astEnhancer.enhance(ast);

  stream_dump(std::cout, enhanced);

  std::cout << std::endl << "Program will exit normally.";
  std::cout << std::endl;
  return 0;
}
