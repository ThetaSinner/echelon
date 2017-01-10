#include <cxxtest/TestSuite.h>
#include <echelon/parser/stage1/tokenizer.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>

#ifdef ECHELON_DEBUG

#include <iostream>

#endif

#ifndef SNIPPETS_DIR
#define SNIPPETS_DIR "../../snippets"
#endif

class ParserSnippetTestSuite : public CxxTest::TestSuite {
private:
  Tokenizer tokenizer;

  bool fileExists(std::string &fname) {
    std::ifstream f(fname.c_str());
    return f.good();
  }

  std::string getFileContents(std::string &fname) {
    if (!fileExists(fname)) {
#ifdef ECHELON_DEBUG
      std::cout << "Missing file " << fname << std::endl;
#endif
      TS_ASSERT(false);
    }

    std::ifstream in(fname.c_str());
    std::stringstream buffer;
    buffer << in.rdbuf();
    static std::regex newline_fix_re("([\r\n]+)");
    return std::regex_replace(buffer.str(), newline_fix_re, "\n");
  }

public:
  ParserSnippetTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));

    loadParserStage1Data();
  }

  void test_Fundamentals_Types(void) {
    std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/types.ech";

    auto res = tokenizer.tokenize(getFileContents(file));

    TS_ASSERT_EQUALS(4, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS("type", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("MyType", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("{", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("}", (*iter)->getData());
  }

  void test_Fundamentals_Comment(void) {
    std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/comment.ech";

    auto res = tokenizer.tokenize(getFileContents(file));

    TS_ASSERT_EQUALS(3, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS(" Single line comment", (*iter)->getData());
    TS_ASSERT_EQUALS(TokenType::SingleLineComment, (*iter)->getTokenType());

    iter++;
    TS_ASSERT_EQUALS("", (*iter)->getData());
    TS_ASSERT_EQUALS(TokenType::MultiLineComment, (*iter)->getTokenType());

    iter++;
    TS_ASSERT_EQUALS("\n * Multi-line comment\n ", (*iter)->getData());
    TS_ASSERT_EQUALS(TokenType::MultiLineComment, (*iter)->getTokenType());
  }

  void test_Fundamentals_Enum(void) {
    std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/enum.ech";

    auto res = tokenizer.tokenize(getFileContents(file));

    TS_ASSERT_EQUALS(6, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS("enum", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("MyEnum", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("{", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("EnumConstOne", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("EnumConstTwo", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("}", (*iter)->getData());
  }

  void test_Fundamentals_variable(void) {
    std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/variable.ech";

    auto res = tokenizer.tokenize(getFileContents(file));

    TS_ASSERT_EQUALS(10, res.size());
    auto iter = res.begin();
    TS_ASSERT_EQUALS("myVar", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("=", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("2", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("myString", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("=", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("Hello, Strings", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("integer", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("myInt", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("=", (*iter)->getData());
    iter++;
    TS_ASSERT_EQUALS("60", (*iter)->getData());
  }
};
