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
      TS_ASSERT_EQUALS("teststr", (*res.begin()) -> getData());
    }

    void testTwoStringsOfLowerCaseLetters(void) {
      auto res = tokenize("test str");

      TS_ASSERT_EQUALS(2, res.size());
      auto iter = res.begin();
      TS_ASSERT_EQUALS("test", (*iter) -> getData());
      iter++;
      TS_ASSERT_EQUALS("str", (*iter) -> getData());
    }

    void testUnderscoreInIdentifier(void) {
      auto res = tokenize("test_str");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("test_str", (*res.begin()) -> getData());
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
      TS_ASSERT_EQUALS("123456", (*res.begin()) -> getData());
    }

    void testTwoStringsOfNumbers(void) {
      auto res = tokenize("123 456");

      TS_ASSERT_EQUALS(2, res.size());
      auto iter = res.begin();
      TS_ASSERT_EQUALS("123", (*iter) -> getData());
      iter++;
      TS_ASSERT_EQUALS("456", (*iter) -> getData());
    }

    void testAssignmentOperator(void) {
      auto res = tokenize("=");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("=", (*res.begin()) -> getData());
    }

    void testEqualityOperator(void) {
      auto res = tokenize("==");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("==", (*res.begin()) -> getData());
    }

    void testEqualityAndAssignmentOperators(void) {
      auto res = tokenize("== = = ==");

      TS_ASSERT_EQUALS(4, res.size());
      auto iter = res.begin();
      TS_ASSERT_EQUALS("==", (*iter) -> getData());
      iter++;
      TS_ASSERT_EQUALS("=", (*iter) -> getData());
      iter++;
      TS_ASSERT_EQUALS("=", (*iter) -> getData());
      iter++;
      TS_ASSERT_EQUALS("==", (*iter) -> getData());
    }
};
