#include <cxxtest/TestSuite.h>

#include <echelon/code-generation/code-generator.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include "../src/code-generation/spider-monkey-js/spider-monkey-js-code-generator.hpp"

class CompilerSpiderMonkeyJSTestSuite : public CxxTest::TestSuite {
    EchelonCompiler compiler;
public:
    CompilerSpiderMonkeyJSTestSuite() {
      compiler.setCodeGenerator(new SpiderMonkeyJSCodeGenerator());
    }

    void testVariableDeclaration(void) {
      auto result = compiler.compile("string my_var = \"my string\"");

      TS_ASSERT_EQUALS("var my_var = \"my string\";", result);
    }
};
