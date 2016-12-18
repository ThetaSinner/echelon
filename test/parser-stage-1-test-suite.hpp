#include <cxxtest/TestSuite.h>

#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>

class ParserTestSuite : public CxxTest::TestSuite
{
public:
    ParserTestSuite() {
      loadParserStage1Data();
    }

    void testStringOfLowerCaseLetters(void) {
      auto res = tokenize("teststr");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("teststr", res.at(0) -> getData());
    }

    void testTwoStringsOfLowerCaseLetters(void) {
      auto res = tokenize("test str");

      TS_ASSERT_EQUALS(2, res.size());
      TS_ASSERT_EQUALS("test", res.at(0) -> getData());
      TS_ASSERT_EQUALS("str", res.at(1) -> getData());
    }

    void testUnderscoreInIdentifier(void) {
      auto res = tokenize("test_str");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("test_str", res.at(0) -> getData());
    }

    void testInvalidCharacterSequence(void) {
      try {
        auto res = tokenize("(*&^^&%JBkjhsadckjh");
      }
      catch (std::exception& e) {
        return;
      }

      TS_ASSERT(false);
    }

    void testStringOfNumbers(void) {
      auto res = tokenize("123456");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("123456", res.at(0) -> getData());
    }

    void testTwoStringsOfNumbers(void) {
      auto res = tokenize("123 456");

      TS_ASSERT_EQUALS(2, res.size());
      TS_ASSERT_EQUALS("123", res.at(0) -> getData());
      TS_ASSERT_EQUALS("456", res.at(1) -> getData());
    }

    void testAssignmentOperator(void) {
      auto res = tokenize("=");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("=", res.at(0) -> getData());
    }

    void testEqualityOperator(void) {
      auto res = tokenize("==");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("==", res.at(0) -> getData());
    }

    void testEqualityAndAssignmentOperators(void) {
      auto res = tokenize("== = = ==");

      TS_ASSERT_EQUALS(4, res.size());
      TS_ASSERT_EQUALS("==", res.at(0) -> getData());
      TS_ASSERT_EQUALS("=", res.at(1) -> getData());
      TS_ASSERT_EQUALS("=", res.at(2) -> getData());
      TS_ASSERT_EQUALS("==", res.at(3) -> getData());
    }
};
