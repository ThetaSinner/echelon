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

  void testVariableDeclaration(void) {
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

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("helloWorld", ast -> getChild(0) -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("byeWorld", ast -> getChild(0) -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChild(1) -> getChildCount());
  }

  void testVariableDeclarationWithType(void) {
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

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("integer", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("helloWorld", ast -> getChild(0) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("byeWorld", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChildCount());
  }

  void testAddTwoStringsAndAssignToVariable(void) {
    std::list<Token*> program;
    program.push_back(new Token("string", TokenTypeEnum::Identifier));
    program.push_back(new Token("my_val", TokenTypeEnum::Identifier));
    program.push_back(new Token("=", TokenTypeEnum::Assign));
    program.push_back(new Token("my string data ", TokenTypeEnum::String));
    program.push_back(new Token("+", TokenTypeEnum::OperatorAdd));
    program.push_back(new Token("other string data", TokenTypeEnum::String));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast -> getType());
    TS_ASSERT_EQUALS(1, ast -> getChildCount());

    TS_ASSERT_EQUALS("my_val", ast -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("string", ast -> getChild(0) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, ast -> getChild(0) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("+", ast -> getChild(0) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperator, ast -> getChild(0) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(2, ast -> getChild(0) -> getChild(1) -> getChildCount());

    TS_ASSERT_EQUALS("my string data ", ast -> getChild(0) -> getChild(1) -> getChild(0) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(0) -> getChildCount());

    TS_ASSERT_EQUALS("other string data", ast -> getChild(0) -> getChild(1) -> getChild(1) -> getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getType());
    TS_ASSERT_EQUALS(0, ast -> getChild(0) -> getChild(1) -> getChild(1) -> getChildCount());
  }
};
