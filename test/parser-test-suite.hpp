#include <cxxtest/TestSuite.h>
#include "elm/parser/tokenizer.hpp"

class ParserTestSuite : public CxxTest::TestSuite
{
public:
    void testStringOfLowerCaseLetters(void) {
      Tokenizer t;
      auto res = t.tokenize("teststr");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("teststr", res.at(0).getData());
    }

    void testTwoStringsOfLowerCaseLetters(void) {
      Tokenizer t;
      auto res = t.tokenize("test str");

      TS_ASSERT_EQUALS(2, res.size());
      TS_ASSERT_EQUALS("test", res.at(0).getData());
      TS_ASSERT_EQUALS("str", res.at(1).getData());
    }

    void testUnderscoreInIdentifier(void) {
      Tokenizer t;
      auto res = t.tokenize("test_str");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("test_str", res.at(0).getData())
    }

    void testUnderscoreAtStartOfIdentifier(void) {
      Tokenizer t;
      try {
        auto res = t.tokenize("_teststr");
      }
      catch (std::exception& e) {
        return;
      }

      TS_ASSERT(false);
    }

    void testStringOfNumbers(void) {
      Tokenizer t;
      auto res = t.tokenize("123456");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("123456", res.at(0).getData());
    }

    void testTwoStringsOfNumbers(void) {
      Tokenizer t;
      auto res = t.tokenize("123 456");

      TS_ASSERT_EQUALS(2, res.size());
      TS_ASSERT_EQUALS("123", res.at(0).getData());
      TS_ASSERT_EQUALS("456", res.at(1).getData());
    }

    void testAssignmentOperator(void) {
      Tokenizer t;
      auto res = t.tokenize("=");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("=", res.at(0).getData());
    }

    void testEqualityOperator(void) {
      Tokenizer t;
      auto res = t.tokenize("==");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("==", res.at(0).getData());
    }

    void testEqualityAndAssignmentOperators(void) {
      Tokenizer t;
      auto res = t.tokenize("== = = ==");

      TS_ASSERT_EQUALS(4, res.size());
      TS_ASSERT_EQUALS("==", res.at(0).getData());
      TS_ASSERT_EQUALS("=", res.at(1).getData());
      TS_ASSERT_EQUALS("=", res.at(2).getData());
      TS_ASSERT_EQUALS("==", res.at(3).getData());
    }
};
