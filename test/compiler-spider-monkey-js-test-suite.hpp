#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/code-generation/code-generator.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include "../src/code-generation/spider-monkey-js/spider-monkey-js-code-generator.hpp"

class CompilerSpiderMonkeyJSTestSuite : public CxxTest::TestSuite {
  EchelonCompiler compiler;
public:
  CompilerSpiderMonkeyJSTestSuite() {
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Off));

    compiler.setCodeGenerator(new SpiderMonkeyJSCodeGenerator());
  }

  void testVariableDeclaration(void) {
    auto result = compiler.compile("string my_var = \"my string\"");

    TS_ASSERT_EQUALS("var my_var = \"my string\";", result);
  }

  void testConditionallyAssignedVariable(void) {
    auto result = compiler.compile("string x\nif (true || false) {\n  x = \"bildo\"\n}\nelse {\n  x = \"winki\"\n}\n");

    // TODO clearly the compiler needs to understand which variables are being declared and which are being assigned.
    TS_ASSERT_EQUALS("var x;if (true || false) {var x = \"bildo\";}else {var x = \"winki\";}", result);
  }
};
