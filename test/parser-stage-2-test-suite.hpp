#include <cxxtest/TestSuite.h>

#include <list>

#include <echelon/parser/parser-data/parser-data-load.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/AstNodeType.hpp>

class ParserStage2TestSuite : public CxxTest::TestSuite
{
private:
  Parser2 parser;

public:
  ParserStage2TestSuite() {
    loadParserData();
  }

  void test_packageDeclaration(void) {
    std::list<Token*> program;
    program.push_back(new Token("package", TokenTypeEnum::Identifier));
    program.push_back(new Token("echelon", TokenTypeEnum::Identifier));
    program.push_back(new Token("::", TokenTypeEnum::StructureOperator));
    program.push_back(new Token("test_package", TokenTypeEnum::Identifier));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());
    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("echelon", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());
    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("test_package", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());
  }

  void test_LongPackageDeclaration(void) {
    std::list<Token*> program;
    program.push_back(new Token("package", TokenTypeEnum::Identifier));
    program.push_back(new Token("test", TokenTypeEnum::Identifier));
    program.push_back(new Token("::", TokenTypeEnum::StructureOperator));
    program.push_back(new Token("pack", TokenTypeEnum::Identifier));
    program.push_back(new Token("::", TokenTypeEnum::StructureOperator));
    program.push_back(new Token("name", TokenTypeEnum::Identifier));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("test", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("pack", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("name", ast -> getChild(0) -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChildCount());
  }

  void test_nestedModule(void) {
    std::list<Token*> program;
    program.push_back(new Token("module", TokenTypeEnum::Identifier));
    program.push_back(new Token("name", TokenTypeEnum::Identifier));
    program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
    program.push_back(new Token("module", TokenTypeEnum::Identifier));
    program.push_back(new Token("nested", TokenTypeEnum::Identifier));
    program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
    program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
    program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("name", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("nested", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());
  }

  void test_SiblingModules() {
    std::list<Token*> program;

    program.push_back(new Token("module", TokenTypeEnum::Identifier));
    program.push_back(new Token("name", TokenTypeEnum::Identifier));
    program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
    program.push_back(new Token("module", TokenTypeEnum::Identifier));
    program.push_back(new Token("nested1", TokenTypeEnum::Identifier));
    program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
    program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
    program.push_back(new Token("module", TokenTypeEnum::Identifier));
    program.push_back(new Token("nested2", TokenTypeEnum::Identifier));
    program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
    program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
    program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("name", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS("nested1", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS("nested2", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChildCount());
  }
};
