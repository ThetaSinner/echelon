#include <cxxtest/TestSuite.h>

#include <list>

#include <echelon/parser/parser-data/parser-data-load.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/parser/token-type-enum.hpp>

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
  }
};
