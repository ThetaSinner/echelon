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
  program.push_back(new Token("this string will only be assigned if 1 and 2 are equal", TokenTypeEnum::String));
  program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  Parser2 parser;
  parser.parse(program);

  std::cout << std::endl << "Program will exit normally.";
  std::cout << std::endl;
  return 0;
}
