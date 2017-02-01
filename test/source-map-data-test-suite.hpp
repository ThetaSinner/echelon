#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/transform/transform-data/type-rule-data-load.hpp>

class SourceMapDataTestSuite : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  SourceMapDataTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));

    loadParserStage1Data();
    loadParserStage2Data();

    loadTypeRuleData();
  }

  void testLineAndCharacterNumbers() {
    auto tokens = compiler.tokenize("integer v = 5\nfunction func() {\n  5 * v\n}");

    TS_ASSERT_EQUALS(13, tokens.size());

    auto iter = tokens.begin();
    TS_ASSERT_EQUALS("integer", (*iter)->getData());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("v", (*iter)->getData());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(8, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("=", (*iter)->getData());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(10, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("5", (*iter)->getData());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(12, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("function", (*iter)->getData());
    TS_ASSERT_EQUALS(1, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("func", (*iter)->getData());
    TS_ASSERT_EQUALS(1, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(9, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("(", (*iter)->getData());
    TS_ASSERT_EQUALS(1, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(13, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS(")", (*iter)->getData());
    TS_ASSERT_EQUALS(1, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(14, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("{", (*iter)->getData());
    TS_ASSERT_EQUALS(1, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(16, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("5", (*iter)->getData());
    TS_ASSERT_EQUALS(2, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(2, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("*", (*iter)->getData());
    TS_ASSERT_EQUALS(2, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(4, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("v", (*iter)->getData());
    TS_ASSERT_EQUALS(2, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(6, (*iter)->getSourceMapData().getCharacterNumber());

    iter++;
    TS_ASSERT_EQUALS("}", (*iter)->getData());
    TS_ASSERT_EQUALS(3, (*iter)->getSourceMapData().getLineNumber());
    TS_ASSERT_EQUALS(0, (*iter)->getSourceMapData().getCharacterNumber());
  }
};
