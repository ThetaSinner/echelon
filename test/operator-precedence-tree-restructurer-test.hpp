#include <cxxtest/TestSuite.h>

#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>

class OperatorPrecedenceTreeRestructurerTest : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  OperatorPrecedenceTreeRestructurerTest() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));
  }

  void testAddThenSubtract() {
    auto expr = compiler.enhance("x + y - z");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, result->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, result->getNodeSubType());

    auto subtract_left = result->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, subtract_left->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, subtract_left->getNodeSubType());

    auto add_left = subtract_left->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, add_left->getNodeType());
    TS_ASSERT_EQUALS("x", add_left->getData());

    auto add_right = subtract_left->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, add_right->getNodeType());
    TS_ASSERT_EQUALS("y", add_right->getData());

    auto subtract_right = result->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, subtract_right->getNodeType());
    TS_ASSERT_EQUALS("z", subtract_right->getData());
  }

  void testAddThenAddThenSubtract() {
    auto expr = compiler.enhance("w + x + y - z");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, result->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, result->getNodeSubType());

    auto subtract_left = result->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, subtract_left->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, subtract_left->getNodeSubType());

    auto add_left = subtract_left->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, add_left->getNodeType());
    TS_ASSERT_EQUALS("w", add_left->getData());

    auto add_right = subtract_left->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, add_right->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, add_right->getNodeSubType());

    auto add_right_left = add_right->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, add_right_left->getNodeType());
    TS_ASSERT_EQUALS("x", add_right_left->getData());

    auto add_right_right = add_right->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, add_right_right->getNodeType());
    TS_ASSERT_EQUALS("y", add_right_right->getData());

    auto subtract_right = result->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, subtract_right->getNodeType());
    TS_ASSERT_EQUALS("z", subtract_right->getData());
  }
};