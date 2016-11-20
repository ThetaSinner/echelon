#include <iostream>
#include <string>
#include <list>

#include "parser/parser-data/parser-data-load.hpp"
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/parser/token.hpp>

#include <echelon/util/stream-dump.hpp>

std::string toString(bool b) {
  return b ? "true" : "false";
}

int main(int argc, char** args) {
  loadParserData();

  Parser2 p2;

  std::list<Token*> program;
  // package echelon::test_package
  program.push_back(new Token("package", TokenTypeEnum::Identifier));
  program.push_back(new Token("echelon", TokenTypeEnum::Identifier));
  program.push_back(new Token("::", TokenTypeEnum::StructureOperator));
  program.push_back(new Token("test_package", TokenTypeEnum::Identifier));

  p2.parse(program);

  std::list<Token*> program2;
  program2.push_back(new Token("package", TokenTypeEnum::Identifier));
  program2.push_back(new Token("test", TokenTypeEnum::Identifier));
  program2.push_back(new Token("::", TokenTypeEnum::StructureOperator));
  program2.push_back(new Token("pack", TokenTypeEnum::Identifier));
  program2.push_back(new Token("::", TokenTypeEnum::StructureOperator));
  program2.push_back(new Token("name", TokenTypeEnum::Identifier));

  p2.parse(program2);

  std::list<Token*> program3;

  program3.push_back(new Token("module", TokenTypeEnum::Identifier));
  program3.push_back(new Token("name", TokenTypeEnum::Identifier));
  program3.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program3.push_back(new Token("module", TokenTypeEnum::Identifier));
  program3.push_back(new Token("nested", TokenTypeEnum::Identifier));
  program3.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  // no content yet.
  program3.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program3.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  p2.parse(program3);

  std::list<Token*> program4;

  program4.push_back(new Token("module", TokenTypeEnum::Identifier));
  program4.push_back(new Token("name", TokenTypeEnum::Identifier));
  program4.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program4.push_back(new Token("module", TokenTypeEnum::Identifier));
  program4.push_back(new Token("nested1", TokenTypeEnum::Identifier));
  program4.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program4.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program4.push_back(new Token("module", TokenTypeEnum::Identifier));
  program4.push_back(new Token("nested2", TokenTypeEnum::Identifier));
  program4.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program4.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program4.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  p2.parse(program4);

  return 0;
}
