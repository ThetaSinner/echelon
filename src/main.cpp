#include <iostream>
#include <map>
#include <list>

#include <echelon/parser/parser-data/parser-data-load.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>
#include <echelon/parser/stage2/parser.hpp>

int main(int argc, char** args) {
  #ifdef ECHELON_DEBUG
  std::cout << "This is a debug build.\n";
  #else
  std::cout << "This is a release build.\n";
  #endif

  loadParserData();

  std::cout << "Parser data loaded." << std::endl;

  std::list<Token*> program;
  program.push_back(new Token("integer", TokenTypeEnum::Identifier));
  program.push_back(new Token("my_val", TokenTypeEnum::Identifier));
  program.push_back(new Token("=", TokenTypeEnum::Assign));
  program.push_back(new Token("helloWorld", TokenTypeEnum::Identifier));
  program.push_back(new Token("(", TokenTypeEnum::ParenO));
  program.push_back(new Token(")", TokenTypeEnum::ParenC));
  program.push_back(new Token("+", TokenTypeEnum::OperatorAdd));
  program.push_back(new Token("byeWorld", TokenTypeEnum::Identifier));
  program.push_back(new Token("(", TokenTypeEnum::ParenO));
  program.push_back(new Token(")", TokenTypeEnum::ParenC));

  Parser2 parser;
  parser.parse(program);

  std::cout << std::endl << "Program will exit normally.";
  std::cout << std::endl;
  return 0;
}
