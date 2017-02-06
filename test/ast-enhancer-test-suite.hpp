#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-function-prototype-node.hpp>

class AstEnhancerTestSuite : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  AstEnhancerTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));
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

    auto var_declare_expression = var_declare->getChild(1);
    TS_ASSERT_EQUALS("", var_declare_expression->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::Expression, var_declare_expression->getNodeType());
    TS_ASSERT_EQUALS(1, var_declare_expression->getChildCount());

    auto var_declare_value = var_declare_expression->getChild(0);
    TS_ASSERT_EQUALS("2", var_declare_value->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::PrimitiveValue, var_declare_value->getNodeType());
    TS_ASSERT_EQUALS(0, var_declare_value->getChildCount());

    auto var_assign = enhanced->getChild(1);
    TS_ASSERT_EQUALS("x", var_assign->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::Variable, var_assign->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Assign, var_assign->getNodeSubType());
    TS_ASSERT_EQUALS(1, var_assign->getChildCount());

    auto var_assign_expression = var_assign->getChild(0);
    TS_ASSERT_EQUALS("", var_assign_expression->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeType::Expression, var_assign_expression->getNodeType());
    TS_ASSERT_EQUALS(1, var_assign_expression->getChildCount());

    auto var_assign_value = var_assign_expression->getChild(0);
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

  void testBlockScope() {
    auto ast = compiler.enhance("package PackageName\ninteger u\nmodule TestModule {\ninteger v = 12\ninteger w = 15\n}");

    auto scope = ((EnhancedAstBlockNode*) ast)->getScope();
    TS_ASSERT(scope != nullptr);
    TS_ASSERT(scope->hasVariable("u"));
    TS_ASSERT(scope->hasModule("TestModule"));
    TS_ASSERT(!scope->hasVariable("v"));
    TS_ASSERT(!scope->hasVariable("w"));

    TS_ASSERT(ast->hasChild(EnhancedAstNodeType::Package));
    auto package = ast->getChild(EnhancedAstNodeType::Package);

    TS_ASSERT(package->hasChild(EnhancedAstNodeType::Module));
    auto module = package->getChild(EnhancedAstNodeType::Module);
    TS_ASSERT_EQUALS("TestModule", module->getData());

    TS_ASSERT(module->hasChild(EnhancedAstNodeType::Block));
    auto block = module->getChild(EnhancedAstNodeType::Block);

    auto blockScope = ((EnhancedAstBlockNode*) block)->getScope();
    TS_ASSERT(blockScope != nullptr);
    TS_ASSERT(blockScope->hasVariable("u"));
    TS_ASSERT(blockScope->hasModule("TestModule"));
    TS_ASSERT(blockScope->hasVariable("v"));
    TS_ASSERT(blockScope->hasVariable("w"));
  }

  void testAttachImplementationToMethod() {
    auto ast = compiler.enhance("type MyType {\n  function getValue()\n}\nfunction MyType::getValue() {\n  5\n}");

    TS_ASSERT(ast->hasChild(EnhancedAstNodeType::CustomType));
    auto custom_type = ast->getChild(EnhancedAstNodeType::CustomType);

    TS_ASSERT_EQUALS("MyType", custom_type->getData());
    TS_ASSERT(custom_type->getChild(EnhancedAstNodeType::Block)->hasChild(EnhancedAstNodeType::Function));
    auto func = custom_type->getChild(EnhancedAstNodeType::Block)->getChild(EnhancedAstNodeType::Function);

    TS_ASSERT_EQUALS("getValue", func->getData());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Prototype, func->getNodeSubType());

    auto prototype = (EnhancedAstFunctionPrototypeNode*) func;
    TS_ASSERT_EQUALS(ast->getChild(EnhancedAstNodeType::Function), prototype->getImpl());
  }

  // TODO this isn't ready yet. Need to be able to map the block and type or determine the type from the block.
  /*
    // example of overloaded functions.
    std::string func_test = "string get_string() {//gay\n} string get_string(integer t) {//af\n}";
    auto enhanced = compiler.enhance(func_test);
  */
};
