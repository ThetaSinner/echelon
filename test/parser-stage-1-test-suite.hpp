#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>

class ParserTestSuite : public CxxTest::TestSuite {
  Tokenizer tokenizer;

public:
  ParserTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));

    loadParserStage1Data();
  }

  void testStringOfLowerCaseLetters(void) {
    auto res = tokenizer.tokenize("teststr");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS("teststr", (*res.begin())->getData());
  }

  void testTwoStringsOfLowerCaseLetters(void) {
    auto res = tokenizer.tokenize("test str");

    TS_ASSERT_EQUALS(2, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS("test", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("str", (*iter)->getData());
  }

  void testUnderscoreInIdentifier(void) {
    auto res = tokenizer.tokenize("test_str");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS("test_str", (*res.begin())->getData());
  }

  void testInvalidCharacterSequence(void) {
    try {
      auto res = tokenizer.tokenize("(*&^^&%JBkjhsadckjh");
    }
    catch (std::exception &e) {
      return;
    }

    TS_ASSERT(false);
  }

  void testStringOfNumbers(void) {
    auto res = tokenizer.tokenize("123456");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS("123456", (*res.begin())->getData());
  }

  void testTwoStringsOfNumbers(void) {
    auto res = tokenizer.tokenize("123 456");

    TS_ASSERT_EQUALS(2, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS("123", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("456", (*iter)->getData());
  }

  void testAssignmentOperator(void) {
    auto res = tokenizer.tokenize("=");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS("=", (*res.begin())->getData());
  }

  void testEqualityOperator(void) {
    auto res = tokenizer.tokenize("==");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS("==", (*res.begin())->getData());
  }

  void testOperatorAdd(void) {
    auto res = tokenizer.tokenize("+");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS("+", res.front()->getData());
    TS_ASSERT_EQUALS(TokenType::AddOperator, res.front()->getTokenType());
  }

  void testEqualityAndAssignmentOperators(void) {
    auto res = tokenizer.tokenize("== = = ==");

    TS_ASSERT_EQUALS(4, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS("==", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("=", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("=", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("==", (*iter)->getData());
  }

  void testStructureOperator(void) {
    auto res = tokenizer.tokenize("::");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS(TokenType::StructureOperator, res.front()->getTokenType());
    TS_ASSERT_EQUALS("::", res.front()->getData());
  }

  void testForwardArrowOperator(void) {
    auto res = tokenizer.tokenize("->");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS(TokenType::ForwardArrowOperator, res.front()->getTokenType());
    TS_ASSERT_EQUALS("->", res.front()->getData());
  }

  void testEllipsisOperator(void) {
    auto res = tokenizer.tokenize("...");

    TS_ASSERT_EQUALS(1, res.size());
    TS_ASSERT_EQUALS(TokenType::EllipsisOperator, res.front()->getTokenType());
    TS_ASSERT_EQUALS("...", res.front()->getData());
  }

  void testEllipsisOperatorWithNumbers(void) {
    auto res = tokenizer.tokenize("1...2");

    TS_ASSERT_EQUALS(3, res.size());

    auto iter = res.begin();
    TS_ASSERT_EQUALS(TokenType::Integer, (*iter)->getTokenType());
    TS_ASSERT_EQUALS("1", (*iter)->getData());

    iter++;
    TS_ASSERT_EQUALS(TokenType::EllipsisOperator, (*iter)->getTokenType());
    TS_ASSERT_EQUALS("...", (*iter)->getData());

    iter++;
    TS_ASSERT_EQUALS(TokenType::Integer, (*iter)->getTokenType());
    TS_ASSERT_EQUALS("2", (*iter)->getData());
  }
};
