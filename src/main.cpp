#include <iostream>
#include <map>
#include <list>
#include <fstream>

#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/util/ast-to-graphviz.hpp>
#include <echelon/transform/type-deduction-engine.hpp>
#include <echelon/util/stream-dump.hpp>
#include <echelon/code-generation/code-generator.hpp>


#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>
#include <echelon/compiler/echelon-compiler.hpp>

int main(int argc, char** args) {
  #ifdef ECHELON_DEBUG
  std::cout << "This is a debug build.\n";
  #else
  std::cout << "This is a release build.\n";
  #endif

  loadParserStage1Data();

  tokenize("9011");
  tokenize("as_df");
  tokenize("asdf fdas");
  tokenize("234.45");
  tokenize("\"happy elf\"");
  tokenize("// healthy comment\n");
  tokenize("/* some \n multi \n line \n comment */");
  tokenize("/**/"); // TODO need to introduce the upper and lower bound concept for this to work.
  tokenize("&&");
  tokenize("||");
  tokenize("{");
  tokenize("}");
  tokenize("if (true || false) {\n  integer x = 5\n}");

  loadParserStage2Data();

  std::cout << "Parser data loaded." << std::endl;

  std::list<Token*> program;
  program.push_back(new Token("if", TokenTypeEnum::Identifier));
  program.push_back(new Token("(", TokenTypeEnum::ParenO));
  program.push_back(new Token("1", TokenTypeEnum::Integer));
  program.push_back(new Token("==", TokenTypeEnum::Equality));
  program.push_back(new Token("2", TokenTypeEnum::Integer));
  program.push_back(new Token(")", TokenTypeEnum::ParenC));
  program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program.push_back(new Token("string", TokenTypeEnum::Identifier));
  program.push_back(new Token("cond_var", TokenTypeEnum::Identifier));
  program.push_back(new Token("=", TokenTypeEnum::Assign));
  program.push_back(new Token("string_one", TokenTypeEnum::String));
  program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program.push_back(new Token("else", TokenTypeEnum::Identifier));
  program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program.push_back(new Token("string", TokenTypeEnum::Identifier));
  program.push_back(new Token("alt_cond_var", TokenTypeEnum::Identifier));
  program.push_back(new Token("=", TokenTypeEnum::Assign));
  program.push_back(new Token("string_two", TokenTypeEnum::String));
  program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  Parser2 parser;
  auto ast = parser.parse(program);

  std::ofstream out("test.gv");
  std::string gv = toGraphviz(ast);
  std::cout << gv << std::endl;
  // dot test.gv -Tjpeg > img.jpeg
  out << gv;
  out.close();

  // Need a good data structure which has all the declared data, types and functions.
  // It needs to be such that it is simple to find out whether something is defined in the current context or parent context
  // for example. Maybe it will be a complementary structure to the existing tree.

  std::list<Token*> program2;
  program2.push_back(new Token("my_str_var", TokenTypeEnum::Identifier));
  program2.push_back(new Token("=", TokenTypeEnum::Assign));
  program2.push_back(new Token("str data", TokenTypeEnum::String));

  auto str_no_type_ast = parser.parse(program2);

  TypeDeductionEngine typeDeductionEngine;
  typeDeductionEngine.deduceTypes(str_no_type_ast);

  stream_dump(std::cout, str_no_type_ast);

  CodeGenerator codeGenerator;
  std::cout << codeGenerator.generate(str_no_type_ast) << std::endl;

  EchelonCompiler compiler;
  std::cout << "Compiler output: " << compiler.compile("test_var = \"test data\"") << std::endl;

  std::cout << std::endl << "Program will exit normally.";
  std::cout << std::endl;
  return 0;
}
