#include <cxxtest/TestSuite.h>
#include "elm/parser/parser.hpp"

class ParserTestSuite : public CxxTest::TestSuite
{
public:
    void testStringOfLowerCaseLetters(void) {
      Parser p;
      auto res = p.parse("teststr");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("teststr", res.at(0));
    }

    void testTwoStringsOfLowerCaseLetters(void) {
      Parser p;
      auto res = p.parse("test str");

      TS_ASSERT_EQUALS(2, res.size());
      TS_ASSERT_EQUALS("test", res.at(0));
      TS_ASSERT_EQUALS("str", res.at(1));
    }

    void testUnderscoreInIdentifier(void) {
      Parser p;
      auto res = p.parse("test_str");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("test_str", res.at(0))
    }

    void testUnderscoreAtStartOfIdentifier(void) {
      Parser p;
      try {
        auto res = p.parse("_teststr");
      }
      catch (std::exception& e) {
        return;
      }

      TS_ASSERT(false);
    }

    void testStringOfNumbers(void) {
      Parser p;
      auto res = p.parse("123456");

      TS_ASSERT_EQUALS(1, res.size());
      TS_ASSERT_EQUALS("123456", res.at(0));
    }

    void testTwoStringsOfNumbers(void) {
      Parser p;
      auto res = p.parse("123 456");

      TS_ASSERT_EQUALS(2, res.size());
      TS_ASSERT_EQUALS("123", res.at(0));
      TS_ASSERT_EQUALS("456", res.at(1));
    }
};
