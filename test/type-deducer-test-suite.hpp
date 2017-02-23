#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/transform/transform-data/type-rule-data-load.hpp>

class TypeDeducerTestSuite : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  TypeDeducerTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));

    loadParserStage1Data();
    loadParserStage2Data();

    loadTypeRuleData();
  }

  void testReturnSumOfTwoIntegers() {
    auto program = compiler.enhance("function get_two_plus_two() {\n  2 + 2}")->getEnhancedAstNode();

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Function));
    auto function = program->getChild(EnhancedAstNodeType::Function);
    TS_ASSERT(function->hasChild(EnhancedAstNodeType::TypeName));
    auto type_name = function->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", type_name->getData());
  }

  void testAssignSumOfTwoIntegers() {
    auto program = compiler.enhance("my_test_variable = 5 + 1")->getEnhancedAstNode();

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Variable));
    auto function = program->getChild(EnhancedAstNodeType::Variable);
    TS_ASSERT(function->hasChild(EnhancedAstNodeType::TypeName));
    auto type_name = function->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", type_name->getData());
  }

  void testAssignSumOfIntegerAndDecimal() {
    auto program = compiler.enhance("my_test_variable = 5 + 1.5")->getEnhancedAstNode();

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Variable));
    auto variable = program->getChild(EnhancedAstNodeType::Variable);
    TS_ASSERT(variable->hasChild(EnhancedAstNodeType::TypeName));
    auto type_name = variable->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("decimal", type_name->getData());
  }

  void testAssignSumOfIntegerAndTypedVariable() {
    auto program = compiler.enhance("integer variable_one = 5\nvariable_one_plus_five = 5 + variable_one")->getEnhancedAstNode();

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Variable));
    TS_ASSERT_EQUALS(2, program->getChildCount());
    auto variable = program->getChild(1);
    TS_ASSERT(variable->hasChild(EnhancedAstNodeType::TypeName));
    auto type_name = variable->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", type_name->getData());
  }

  void testAssignSumOfIntegerAndDeterminedTypedVariable() {
    auto program = compiler.enhance("variable_one = 5\nvariable_one_plus_five = 5 + variable_one")->getEnhancedAstNode();

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Variable));
    TS_ASSERT_EQUALS(2, program->getChildCount());

    auto var = program->getChild(0);
    TS_ASSERT(var->hasChild(EnhancedAstNodeType::TypeName));
    auto var_type_name = var->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", var_type_name->getData());
    
    auto variable = program->getChild(1);
    TS_ASSERT(variable->hasChild(EnhancedAstNodeType::TypeName));
    auto variable_type_name = variable->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", variable_type_name->getData());
  }

  void testReturnProductOfIntegerAndVariableFromHigherScope() {
    auto program = compiler.enhance("integer x = 3\nfunction getFiveTimesX() {\n  5 * x\n}")->getEnhancedAstNode();

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Variable));
    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Function));

    auto function = program->getChild(EnhancedAstNodeType::Function);
    TS_ASSERT(function->hasChild(EnhancedAstNodeType::TypeName));
    TS_ASSERT_EQUALS("integer", function->getChild(EnhancedAstNodeType::TypeName)->getData());
  }

  void testDeduceTypeFromFunctionCallWithFunctionsNotDeclaredYet() {
    auto ast = compiler.enhance("val = foo() * foo() + bar() * bar() function foo() {5} function bar() {2}")->getEnhancedAstNode();

    TS_ASSERT(ast->hasChild(EnhancedAstNodeType::Variable));
    auto variable = ast->getChild(EnhancedAstNodeType::Variable);

    TS_ASSERT(variable->hasChild(EnhancedAstNodeType::TypeName));
    auto typeName = variable->getChild(EnhancedAstNodeType::TypeName);

    TS_ASSERT_EQUALS("integer", typeName->getData());
  }
};