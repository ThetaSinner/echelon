#include <iostream>
#include <map>
#include <list>

#include <echelon/parser/parser-data/parser-data-load.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>
#include <echelon/parser/stage2/parser.hpp>

std::string toString(bool b) {
  return b ? "true" : "false";
}

int main(int argc, char** args) {
  std::cout << "Clean main.\n";

  loadParserData();

  std::list<Token*> program;
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
  //parser.parse(program);

  std::cout << std::endl;
  return 0;
}
