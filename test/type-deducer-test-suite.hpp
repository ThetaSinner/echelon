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
    auto program = compiler.enhance("function get_two_plus_two() {\n  2 + 2}");

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Function));
    auto function = program->getChild(EnhancedAstNodeType::Function);
    TS_ASSERT(function->hasChild(EnhancedAstNodeType::TypeName));
    auto type_name = function->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", type_name->getData());
  }

  void testAssignSumOfTwoIntegers() {
    auto program = compiler.enhance("my_test_variable = 5 + 1");

    TS_ASSERT(program->hasChild(EnhancedAstNodeType::Variable));
    auto function = program->getChild(EnhancedAstNodeType::Variable);
    TS_ASSERT(function->hasChild(EnhancedAstNodeType::TypeName));
    auto type_name = function->getChild(EnhancedAstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", type_name->getData());
  }
};