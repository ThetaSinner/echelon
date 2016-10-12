#include <cxxtest/TestSuite.h>
#include "elm/parser/parser.hpp"

#include <fstream>
#include <sstream>
#include <string>

#ifndef SNIPPETS_DIR
  #define SNIPPETS_DIR "../../snippets"
#endif

class ParserSnippetTestSuite : public CxxTest::TestSuite
{
private:
  bool fileExists(std::string& fname) {
    std::ifstream f(fname.c_str());
    return f.good();
  }

  std::string getFileContents(std::string& fname) {
    if (!fileExists(fname)) {
        std::cout << "Missing file " << fname << std::endl;
        TS_ASSERT(false);
    }

    std::ifstream in(fname.c_str());
    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
  }

public:
    void test_Fundamentals_Types(void) {
      std::string file = std::string(SNIPPETS_DIR) + "/fundamentals/types.elm";

      Parser p;
      auto res = p.parse(getFileContents(file));

      TS_ASSERT_EQUALS(4, res.size());
      TS_ASSERT_EQUALS("type", res.at(0));
      TS_ASSERT_EQUALS("MyType", res.at(1));
      TS_ASSERT_EQUALS("{", res.at(2));
      TS_ASSERT_EQUALS("}", res.at(3));
    }
};
