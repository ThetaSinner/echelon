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

  void testSubtractAddSubtract() {
    auto expr = compiler.enhance("w - x + y - z");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto top_subtract = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, top_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, top_subtract->getNodeSubType());

    auto w = top_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, w->getNodeType());
    TS_ASSERT_EQUALS("w", w->getData());

    auto second_subtract = top_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, second_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, second_subtract->getNodeSubType());

    auto z = second_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, z->getNodeType());
    TS_ASSERT_EQUALS("z", z->getData());

    auto add = second_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, add->getNodeSubType());

    auto x = add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, x->getNodeType());
    TS_ASSERT_EQUALS("x", x->getData());

    auto y = add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, y->getNodeType());
    TS_ASSERT_EQUALS("y", y->getData());
  }

  void testSubtractMultiplyAdd() {
    auto expr = compiler.enhance("w - x * y + z");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto subtract = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, subtract->getNodeSubType());

    auto w = subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, w->getNodeType());
    TS_ASSERT_EQUALS("w", w->getData());

    auto add = subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, add->getNodeSubType());

    auto z = add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, z->getNodeType());
    TS_ASSERT_EQUALS("z", z->getData());

    auto multiply = add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, multiply->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Multiply, multiply->getNodeSubType());

    auto x = multiply->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, x->getNodeType());
    TS_ASSERT_EQUALS("x", x->getData());

    auto y = multiply->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, y->getNodeType());
    TS_ASSERT_EQUALS("y", y->getData());
  }

  void testMultiplyDivide() {
    auto expr = compiler.enhance("x / y * z");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto multiply = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, multiply->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Multiply, multiply->getNodeSubType());

    auto z = multiply->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, z->getNodeType());
    TS_ASSERT_EQUALS("z", z->getData());
    
    auto divide = multiply->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, divide->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Divide, divide->getNodeSubType());

    auto x = divide->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, x->getNodeType());
    TS_ASSERT_EQUALS("x", x->getData());

    auto y = divide->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, y->getNodeType());
    TS_ASSERT_EQUALS("y", y->getData());
  }

  void testMultiplyExpressionGroup() {
    auto expr = compiler.enhance("a * (b + c - d)");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto multiply = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, multiply->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Multiply, multiply->getNodeSubType());

    auto a = multiply->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, a->getNodeType());
    TS_ASSERT_EQUALS("a", a->getData());

    auto expression_group = multiply->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, expression_group->getNodeType());

    auto subtract = expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, subtract->getNodeSubType());

    auto d = subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, d->getNodeType());
    TS_ASSERT_EQUALS("d", d->getData());

    auto add = subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, add->getNodeSubType());

    auto b = add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, b->getNodeType());
    TS_ASSERT_EQUALS("b", b->getData());

    auto c = add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, c->getNodeType());
    TS_ASSERT_EQUALS("c", c->getData());
  }

  void testMultiplyExpressionGroups() {
    auto expr = compiler.enhance("(a - b + c) * (d + e - f)");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto multiply = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, multiply->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Multiply, multiply->getNodeSubType());

    auto left_expression_group = multiply->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, left_expression_group->getNodeType());

    auto left_subtract = left_expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, left_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, left_subtract->getNodeSubType());

    auto a = left_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, a->getNodeType());
    TS_ASSERT_EQUALS("a", a->getData());

    auto left_add = left_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, left_add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, left_add->getNodeSubType());

    auto b = left_add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, b->getNodeType());
    TS_ASSERT_EQUALS("b", b->getData());

    auto c = left_add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, c->getNodeType());
    TS_ASSERT_EQUALS("c", c->getData());

    auto right_expression_group = multiply->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, right_expression_group->getNodeType());

    auto right_subtract = right_expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, right_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, right_subtract->getNodeSubType());

    auto f = right_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, f->getNodeType());
    TS_ASSERT_EQUALS("f", f->getData());

    auto right_add = right_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, right_add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, right_add->getNodeSubType());

    auto d = right_add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, d->getNodeType());
    TS_ASSERT_EQUALS("d", d->getData());

    auto e = right_add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, e->getNodeType());
    TS_ASSERT_EQUALS("e", e->getData());
  }

  void testTopLevelExpressionGroup() {
    auto expr = compiler.enhance("(a + b - c)");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto expression_group = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, expression_group->getNodeType());

    auto subtract = expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, subtract->getNodeSubType());

    auto c = subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, c->getNodeType());
    TS_ASSERT_EQUALS("c", c->getData());

    auto add = subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, add->getNodeSubType());

    auto a = add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, a->getNodeType());
    TS_ASSERT_EQUALS("a", a->getData());

    auto b = add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, b->getNodeType());
    TS_ASSERT_EQUALS("b", b->getData());
  }

  void testMultipleExpressionGroups() {
    auto expr = compiler.enhance("(a + b - c) / (d + e - f) * (g + h - i)");
    auto result = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    auto multiply = result;
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, multiply->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Multiply, multiply->getNodeSubType());

    auto right_expression_group = multiply->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, right_expression_group->getNodeType());

    auto right_subtract = right_expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, right_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, right_subtract->getNodeSubType());

    auto i = right_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, i->getNodeType());
    TS_ASSERT_EQUALS("i", i->getData());

    auto right_add = right_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, right_add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, right_add->getNodeSubType());

    auto g = right_add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, g->getNodeType());
    TS_ASSERT_EQUALS("g", g->getData());

    auto h = right_add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, h->getNodeType());
    TS_ASSERT_EQUALS("h", h->getData());

    auto divide = multiply->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, divide->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Divide, divide->getNodeSubType());

    auto left_left_expression_group = divide->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, left_left_expression_group->getNodeType());

    auto left_left_subtract = left_left_expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, left_left_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, left_left_subtract->getNodeSubType());

    auto c = left_left_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, c->getNodeType());
    TS_ASSERT_EQUALS("c", c->getData());

    auto left_left_add = left_left_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, left_left_add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, left_left_add->getNodeSubType());

    auto a = left_left_add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, a->getNodeType());
    TS_ASSERT_EQUALS("a", a->getData());

    auto b = left_left_add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, b->getNodeType());
    TS_ASSERT_EQUALS("b", b->getData());

    auto left_right_expression_group = divide->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::ExpressionGroup, left_right_expression_group->getNodeType());

    auto left_right_subtract = left_right_expression_group->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, left_right_subtract->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Subtract, left_right_subtract->getNodeSubType());

    auto f = left_right_subtract->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, f->getNodeType());
    TS_ASSERT_EQUALS("f", f->getData());

    auto left_right_add = left_right_subtract->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::BinaryOperator, left_right_add->getNodeType());
    TS_ASSERT_EQUALS(EnhancedAstNodeSubType::Add, left_right_add->getNodeSubType());

    auto d = left_right_add->getChild(0);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, d->getNodeType());
    TS_ASSERT_EQUALS("d", d->getData());

    auto e = left_right_add->getChild(1);
    TS_ASSERT_EQUALS(EnhancedAstNodeType::VariableValue, e->getNodeType());
    TS_ASSERT_EQUALS("e", e->getData());
  }
};