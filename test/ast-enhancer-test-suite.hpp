#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/compiler/echelon-compiler.hpp>

class AstEnhancerTestSuite : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  AstEnhancerTestSuite() {
    // Turn off logging.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Off));
  }

  // TODO create test for declare without assign.
  void testVariableDeclareAndAssign(void) {
    auto enhanced = compiler.enhance("integer x = 2\nx = 3");

    TS_ASSERT_EQUALS("root", enhanced->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::Program, enhanced->getNodeType());
    TS_ASSERT_EQUALS(2, enhanced->getChildCount());

    auto var_declare = enhanced->getChild(0);
    TS_ASSERT_EQUALS("x", var_declare->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::Variable, var_declare->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Declaration, var_declare->getNodeSubType());
    TS_ASSERT_EQUALS(2, var_declare->getChildCount());

    auto var_declare_type = var_declare->getChild(0);
    TS_ASSERT_EQUALS("integer", var_declare_type->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::TypeName, var_declare_type->getNodeType());
    TS_ASSERT_EQUALS(0, var_declare_type->getChildCount());

    auto var_declare_value = var_declare->getChild(1);
    TS_ASSERT_EQUALS("2", var_declare_value->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::PrimitiveValue, var_declare_value->getNodeType());
    TS_ASSERT_EQUALS(0, var_declare_value->getChildCount());

    auto var_assign = enhanced->getChild(1);
    TS_ASSERT_EQUALS("x", var_assign->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::Variable, var_assign->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Assign, var_assign->getNodeSubType());
    TS_ASSERT_EQUALS(1, var_assign->getChildCount());

    auto var_assign_value = var_assign->getChild(0);
    TS_ASSERT_EQUALS("3", var_assign_value->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::PrimitiveValue, var_assign_value->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Integer, var_assign_value->getNodeSubType());
    TS_ASSERT_EQUALS(0, var_assign_value->getChildCount());
  }

  void testEnhancePackage() {
    auto ast = compiler.enhance("package Test::Package::Name\ninteger x = 5");

    // Notice that we need something else working so we can check the rest of the program is attached at the bottom of the
    // package structure.

    TS_ASSERT(ast->hasChild(EnhancedAstNodeType::Package));
    auto test_package = ast->getChild(EnhancedAstNodeType::Package);
    TS_ASSERT_EQUALS("Test", test_package->getData());

    TS_ASSERT(test_package->hasChild(EnhancedAstNodeType::Package));
    auto package_package = test_package->getChild(EnhancedAstNodeType::Package);
    TS_ASSERT_EQUALS("Package", package_package->getData());

    TS_ASSERT(package_package->hasChild(EnhancedAstNodeType::Package));
    auto name_package = package_package->getChild(EnhancedAstNodeType::Package);
    TS_ASSERT_EQUALS("Name", name_package->getData());

    TS_ASSERT(name_package->hasChild(EnhancedAstNodeType::Variable));

    // don't care about the variable. Just that it exists.
  }

  // TODO this isn't ready yet. Need to be able to map the block and type or determine the type from the block.
  /*
    // example of overloaded functions.
    std::string func_test = "string get_string() {//gay\n} string get_string(integer t) {//af\n}";
    auto enhanced = compiler.enhance(func_test);
  */
};
