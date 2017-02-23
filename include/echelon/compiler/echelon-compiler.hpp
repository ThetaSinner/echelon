#ifndef ECHELON_COMPILER_HPP_INCLUDED
#define ECHELON_COMPILER_HPP_INCLUDED

#include <string>

#include <echelon/parser/stage2/parser.hpp>
#include <echelon/code-generation/code-generator.hpp>
#include <echelon/transform/type-deducer.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>
#include <echelon/transform/ast-enhancer.hpp>
#include <echelon/transform/ast-enhancer-output.hpp>

class EchelonCompiler {
  Tokenizer tokenizer;
  Parser2 parser;
  AstEnhancer astEnhancer;
  TypeDeducer typeDeducer;
  CodeGenerator *codeGenerator;

  std::list<Token *> tokenizeInternal(std::string input);
  AstEnhancerOutput* enhanceInternal(std::string input);
public:
  EchelonCompiler();

  void setCodeGenerator(CodeGenerator *codeGenerator);

  std::string compile(std::string input);

  std::list<Token *> tokenize(std::string input);

  AstNode *parse(std::list<Token *> input);
  AstNode *parse(std::string input);

  AstEnhancerOutput* enhance(AstNode *input);
  AstEnhancerOutput* enhance(std::string input);
  // TODO type engine needs to be converted to take enhanced nodes.
  // TODO generator needs to be converted to take enhanced nodes.
  // TODO code generation needs to be converted to take enhanced nodes.
};

#endif
