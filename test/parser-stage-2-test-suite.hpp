#include <cxxtest/TestSuite.h>

#include <list>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/parser/stage1/token.hpp>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/util/to-string.hpp>

class ParserStage2TestSuite : public CxxTest::TestSuite {
private:
  Parser2 parser;
  Tokenizer tokenizer;
  EchelonCompiler compiler;

public:
  ParserStage2TestSuite() {
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Off));

    loadParserStage1Data();
    loadParserStage2Data();
  }

  void test_packageDeclaration(void) {
    auto ast = compiler.parse("package echelon::test_package");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto echelon_package = ast->getChild(0);
    TS_ASSERT_EQUALS("echelon", echelon_package->getData());
    TS_ASSERT_EQUALS(AstNodeType::Package, echelon_package->getType());
    TS_ASSERT_EQUALS(1, echelon_package->getChildCount());

    auto test_package = echelon_package->getChild(0);
    TS_ASSERT_EQUALS("test_package", test_package->getData());
    TS_ASSERT_EQUALS(AstNodeType::Package, test_package->getType());
    TS_ASSERT_EQUALS(0, test_package->getChildCount());
  }

  void test_LongPackageDeclaration(void) {
    std::list<Token *> program;
    program.push_back(new Token("package", TokenType::Identifier));
    program.push_back(new Token("test", TokenType::Identifier));
    program.push_back(new Token("::", TokenType::StructureOperator));
    program.push_back(new Token("pack", TokenType::Identifier));
    program.push_back(new Token("::", TokenType::StructureOperator));
    program.push_back(new Token("name", TokenType::Identifier));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS("test", ast->getChild(0)->getData());
    TS_ASSERT_EQUALS(1, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS("pack", ast->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(1, ast->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Package, ast->getChild(0)->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS("name", ast->getChild(0)->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChild(0)->getChildCount());
  }

  void test_nestedModule(void) {
    std::list<Token *> program;
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("name", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("nested", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS("name", ast->getChild(0)->getData());
    TS_ASSERT_EQUALS(1, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS("nested", ast->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChildCount());
  }

  void test_SiblingModules() {
    std::list<Token *> program;

    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("name", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("nested1", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));
    program.push_back(new Token("module", TokenType::Identifier));
    program.push_back(new Token("nested2", TokenType::Identifier));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS("name", ast->getChild(0)->getData());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS("nested1", ast->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Module, ast->getChild(0)->getChild(1)->getType());
    TS_ASSERT_EQUALS("nested2", ast->getChild(0)->getChild(1)->getData());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChildCount());
  }

  void testVariableDeclaration(void) {
    std::list<Token *> program;
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assignment));
    program.push_back(new Token("helloWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenthesisOpen));
    program.push_back(new Token(")", TokenType::ParenthesisClose));
    program.push_back(new Token("+", TokenType::AddOperator));
    program.push_back(new Token("byeWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenthesisOpen));
    program.push_back(new Token(")", TokenType::ParenthesisClose));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    AstNode *var_node = ast->getChild(0);
    TS_ASSERT_EQUALS("my_val", var_node->getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, var_node->getType());
    TS_ASSERT_EQUALS(1, var_node->getChildCount());

    TS_ASSERT_EQUALS("+", var_node->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, var_node->getChild(0)->getType());
    TS_ASSERT_EQUALS(2, var_node->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("helloWorld", var_node->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, var_node->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, var_node->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("byeWorld", var_node->getChild(0)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, var_node->getChild(0)->getChild(1)->getType());
    TS_ASSERT_EQUALS(0, var_node->getChild(0)->getChild(1)->getChildCount());
  }

  void testVariableDeclarationWithType(void) {
    std::list<Token *> program;
    program.push_back(new Token("integer", TokenType::Identifier));
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assignment));
    program.push_back(new Token("helloWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenthesisOpen));
    program.push_back(new Token(")", TokenType::ParenthesisClose));
    program.push_back(new Token("+", TokenType::AddOperator));
    program.push_back(new Token("byeWorld", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenthesisOpen));
    program.push_back(new Token(")", TokenType::ParenthesisClose));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS("my_val", ast->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("integer", ast->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::TypeName, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("+", ast->getChild(0)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, ast->getChild(0)->getChild(1)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChild(1)->getChildCount());

    TS_ASSERT_EQUALS("helloWorld", ast->getChild(0)->getChild(1)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast->getChild(0)->getChild(1)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("byeWorld", ast->getChild(0)->getChild(1)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, ast->getChild(0)->getChild(1)->getChild(1)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(1)->getChildCount());
  }

  void testAddTwoStringsAndAssignToVariable(void) {
    std::list<Token *> program;
    program.push_back(new Token("string", TokenType::Identifier));
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assignment));
    program.push_back(new Token("my string data ", TokenType::String));
    program.push_back(new Token("+", TokenType::AddOperator));
    program.push_back(new Token("other string data", TokenType::String));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS("my_val", ast->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("string", ast->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::TypeName, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("+", ast->getChild(0)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, ast->getChild(0)->getChild(1)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChild(1)->getChildCount());

    TS_ASSERT_EQUALS("my string data ", ast->getChild(0)->getChild(1)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast->getChild(0)->getChild(1)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("other string data", ast->getChild(0)->getChild(1)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast->getChild(0)->getChild(1)->getChild(1)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(1)->getChildCount());
  }

  // TODO use the parse on the compiler
  void testAddThreeStringsAndAssignToVariable(void) {
    std::list<Token *> program;
    program.push_back(new Token("string", TokenType::Identifier));
    program.push_back(new Token("my_val", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assignment));
    program.push_back(new Token("my string data ", TokenType::String));
    program.push_back(new Token("+", TokenType::AddOperator));
    program.push_back(new Token("other string data", TokenType::String));
    program.push_back(new Token("+", TokenType::AddOperator));
    program.push_back(new Token("third string data", TokenType::String));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS("my_val", ast->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("string", ast->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::TypeName, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("+", ast->getChild(0)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, ast->getChild(0)->getChild(1)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChild(1)->getChildCount());

    TS_ASSERT_EQUALS("my string data ", ast->getChild(0)->getChild(1)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast->getChild(0)->getChild(1)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("+", ast->getChild(0)->getChild(1)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, ast->getChild(0)->getChild(1)->getChild(1)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChild(1)->getChild(1)->getChildCount());

    TS_ASSERT_EQUALS("other string data", ast->getChild(0)->getChild(1)->getChild(1)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast->getChild(0)->getChild(1)->getChild(1)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(1)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("third string data", ast->getChild(0)->getChild(1)->getChild(1)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, ast->getChild(0)->getChild(1)->getChild(1)->getChild(1)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(1)->getChild(1)->getChild(1)->getChildCount());
  }

  void testIfStatementWithEmptyBlock(void) {
    std::list<Token *> program;
    program.push_back(new Token("if", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenthesisOpen));
    program.push_back(new Token("1", TokenType::Integer));
    program.push_back(new Token("==", TokenType::LogicalEquality));
    program.push_back(new Token("2", TokenType::Integer));
    program.push_back(new Token(")", TokenType::ParenthesisClose));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto branches = ast->getChild(0);
    TS_ASSERT_EQUALS(AstNodeType::Branches, branches->getType());
    TS_ASSERT_EQUALS(1, branches->getChildCount());

    auto _if = ast->getChild(0)->getChild(0);
    TS_ASSERT_EQUALS(AstNodeType::If, _if->getType());
    TS_ASSERT_EQUALS(1, _if->getChildCount());

    auto eq_op = ast->getChild(0)->getChild(0)->getChild(0);
    TS_ASSERT_EQUALS(AstNodeType::EqualityOperator, eq_op->getType());
    TS_ASSERT_EQUALS(2, eq_op->getChildCount());

    auto int_1 = ast->getChild(0)->getChild(0)->getChild(0)->getChild(0);
    TS_ASSERT_EQUALS("1", int_1->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, int_1->getType());
    TS_ASSERT_EQUALS(0, int_1->getChildCount());

    auto int_2 = ast->getChild(0)->getChild(0)->getChild(0)->getChild(1);
    TS_ASSERT_EQUALS("2", int_2->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, int_2->getType());
    TS_ASSERT_EQUALS(0, int_2->getChildCount());
  }

  void testIfStatement(void) {
    std::list<Token *> program;
    program.push_back(new Token("if", TokenType::Identifier));
    program.push_back(new Token("(", TokenType::ParenthesisOpen));
    program.push_back(new Token("1", TokenType::Integer));
    program.push_back(new Token("==", TokenType::LogicalEquality));
    program.push_back(new Token("2", TokenType::Integer));
    program.push_back(new Token(")", TokenType::ParenthesisClose));
    program.push_back(new Token("{", TokenType::BlockDelimiterOpen));
    program.push_back(new Token("string", TokenType::Identifier));
    program.push_back(new Token("cond_var", TokenType::Identifier));
    program.push_back(new Token("=", TokenType::Assignment));
    program.push_back(new Token("this string will only be assigned if 1 and 2 are equal", TokenType::String));
    program.push_back(new Token("}", TokenType::BlockDelimiterClose));

    auto ast = parser.parse(program);

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::Branches, ast->getChild(0)->getType());
    TS_ASSERT_EQUALS(1, ast->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::If, ast->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS(AstNodeType::EqualityOperator, ast->getChild(0)->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(2, ast->getChild(0)->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("1", ast->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, ast->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChild(0)->getChild(0)->getChildCount());

    TS_ASSERT_EQUALS("2", ast->getChild(0)->getChild(0)->getChild(0)->getChild(1)->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, ast->getChild(0)->getChild(0)->getChild(0)->getChild(1)->getType());
    TS_ASSERT_EQUALS(0, ast->getChild(0)->getChild(0)->getChild(0)->getChild(1)->getChildCount());

    auto block = ast->getChild(0)->getChild(0)->getChild(1);
    TS_ASSERT_EQUALS("", block->getData());
    TS_ASSERT_EQUALS(AstNodeType::Block, block->getType());
    TS_ASSERT_EQUALS(1, block->getChildCount());

    auto cond_var = block->getChild(0);
    TS_ASSERT_EQUALS("cond_var", cond_var->getData());
    TS_ASSERT_EQUALS(AstNodeType::Variable, cond_var->getType());
    TS_ASSERT_EQUALS(2, cond_var->getChildCount());

    auto cond_var_type = cond_var->getChild(0);
    TS_ASSERT_EQUALS("string", cond_var_type->getData());
    TS_ASSERT_EQUALS(AstNodeType::TypeName, cond_var_type->getType());
    TS_ASSERT_EQUALS(0, cond_var_type->getChildCount());

    auto cond_var_data = cond_var->getChild(1);
    TS_ASSERT_EQUALS("this string will only be assigned if 1 and 2 are equal", cond_var_data->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, cond_var_data->getType());
    TS_ASSERT_EQUALS(0, cond_var_data->getChildCount());
  }

  void testFunctionDeclaration(void) {
    auto ast = parser.parse(tokenizer.tokenize("function my_func(string t, x) -> integer {// test comment\n}"));

    TS_ASSERT(ast->hasChild(AstNodeType::Function));
    auto function = ast->getChild(AstNodeType::Function);
    TS_ASSERT_EQUALS("my_func", function->getData());
    TS_ASSERT_EQUALS(3, function->getChildCount());

    auto return_type = function->getChild(AstNodeType::TypeName);
    TS_ASSERT_EQUALS("integer", return_type->getData());
    TS_ASSERT_EQUALS(0, return_type->getChildCount());

    auto param_definitions = function->getChild(AstNodeType::FunctionParamDefinitions);
    TS_ASSERT_EQUALS(2, param_definitions->getChildCount());

    auto typed_param_definition = param_definitions->getChild(0);
    TS_ASSERT_EQUALS("t", typed_param_definition->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionParamDefinition, typed_param_definition->getType());
    TS_ASSERT_EQUALS(1, typed_param_definition->getChildCount());

    TS_ASSERT_EQUALS("string", typed_param_definition->getChild(0)->getData());
    TS_ASSERT_EQUALS(AstNodeType::TypeName, typed_param_definition->getChild(0)->getType());
    TS_ASSERT_EQUALS(0, typed_param_definition->getChild(0)->getChildCount());

    auto untyped_param_definition = param_definitions->getChild(1);
    TS_ASSERT_EQUALS("x", untyped_param_definition->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionParamDefinition, untyped_param_definition->getType());
    TS_ASSERT_EQUALS(0, untyped_param_definition->getChildCount());

    auto block = function->getChild(AstNodeType::Block);
    TS_ASSERT_EQUALS(1, block->getChildCount());

    // don't care about the block contents.
  }

  void testFunctionCall() {
    auto ast = parser.parse(tokenizer.tokenize("my_func()"));

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto function_call = ast->getChild(0);
    TS_ASSERT_EQUALS("my_func", function_call->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, function_call->getType());
    TS_ASSERT_EQUALS(0, function_call->getChildCount());
  }

  void testFunctionCallWithArgs() {
    auto ast = parser.parse(tokenizer.tokenize("my_func(\"a string\", 0.5)"));

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto function_call = ast->getChild(0);
    TS_ASSERT_EQUALS("my_func", function_call->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCall, function_call->getType());
    TS_ASSERT_EQUALS(1, function_call->getChildCount());

    auto call_params = function_call->getChild(0);
    TS_ASSERT_EQUALS("", call_params->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCallParams, call_params->getType());
    TS_ASSERT_EQUALS(2, call_params->getChildCount());

    auto call_param_one = call_params->getChild(0);
    TS_ASSERT_EQUALS("", call_param_one->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCallParam, call_param_one->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto call_param_one_val = call_param_one->getChild(0);
    TS_ASSERT_EQUALS("a string", call_param_one_val->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, call_param_one_val->getType());
    TS_ASSERT_EQUALS(0, call_param_one_val->getChildCount());

    auto call_param_two = call_params->getChild(1);
    TS_ASSERT_EQUALS("", call_param_two->getData());
    TS_ASSERT_EQUALS(AstNodeType::FunctionCallParam, call_param_two->getType());
    TS_ASSERT_EQUALS(1, call_param_two->getChildCount());

    auto call_param_two_val = call_param_two->getChild(0);
    TS_ASSERT_EQUALS("0.5", call_param_two_val->getData());
    TS_ASSERT_EQUALS(AstNodeType::Decimal, call_param_two_val->getType());
    TS_ASSERT_EQUALS(0, call_param_two_val->getChildCount());
  }

  void testExpressionStatement() {
    auto ast = compiler.parse("2 + \"a string\"");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto plus = ast->getChild(0);
    TS_ASSERT_EQUALS("+", plus->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, plus->getType());
    TS_ASSERT_EQUALS(2, plus->getChildCount());

    auto integer = plus->getChild(0);
    TS_ASSERT_EQUALS("2", integer->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, integer->getType());
    TS_ASSERT_EQUALS(0, integer->getChildCount());

    auto string = plus->getChild(1);
    TS_ASSERT_EQUALS("a string", string->getData());
    TS_ASSERT_EQUALS(AstNodeType::String, string->getType());
    TS_ASSERT_EQUALS(0, string->getChildCount());
  }

  void testEnum() {
    auto ast = compiler.parse("enum MyEnum {\n  EnumConstOne\n  EnumConstTwo\n}");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto _enum = ast->getChild(0);
    TS_ASSERT_EQUALS("MyEnum", _enum->getData());
    TS_ASSERT_EQUALS(AstNodeType::Enum, _enum->getType());
    TS_ASSERT_EQUALS(1, _enum->getChildCount());

    auto enum_constants = _enum->getChild(0);
    TS_ASSERT_EQUALS("", enum_constants->getData());
    TS_ASSERT_EQUALS(AstNodeType::EnumConstants, enum_constants->getType());
    TS_ASSERT_EQUALS(2, enum_constants->getChildCount());

    auto enum_const_one = enum_constants->getChild(0);
    TS_ASSERT_EQUALS("EnumConstOne", enum_const_one->getData());
    TS_ASSERT_EQUALS(AstNodeType::EnumConstant, enum_const_one->getType());
    TS_ASSERT_EQUALS(0, enum_const_one->getChildCount());

    auto enum_const_two = enum_constants->getChild(1);
    TS_ASSERT_EQUALS("EnumConstTwo", enum_const_two->getData());
    TS_ASSERT_EQUALS(AstNodeType::EnumConstant, enum_const_two->getType());
    TS_ASSERT_EQUALS(0, enum_const_two->getChildCount());
  }

  void testUntilLoop() {
    auto ast = compiler.parse("until (true || false) {\n  integer x = 3\n}");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto until = ast->getChild(0);
    TS_ASSERT_EQUALS("", until->getData());
    TS_ASSERT_EQUALS(AstNodeType::Until, until->getType());
    TS_ASSERT_EQUALS(2, until->getChildCount());

    auto condition = until->getChild(AstNodeType::Condition);
    TS_ASSERT_EQUALS("", condition->getData());
    TS_ASSERT_EQUALS(AstNodeType::Condition, condition->getType());
    TS_ASSERT_EQUALS(1, condition->getChildCount());

    // don't care about the condition.

    auto block = until->getChild(AstNodeType::Block);
    TS_ASSERT_EQUALS("", block->getData());
    TS_ASSERT_EQUALS(AstNodeType::Block, block->getType());
    TS_ASSERT_EQUALS(1, block->getChildCount());

    // don't care about the block contents
  }

  void testWhileLoop() {
    auto ast = compiler.parse("while (true || false) {\n  integer x = 3\n}");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto until = ast->getChild(0);
    TS_ASSERT_EQUALS("", until->getData());
    TS_ASSERT_EQUALS(AstNodeType::While, until->getType());
    TS_ASSERT_EQUALS(2, until->getChildCount());

    auto condition = until->getChild(AstNodeType::Condition);
    TS_ASSERT_EQUALS("", condition->getData());
    TS_ASSERT_EQUALS(AstNodeType::Condition, condition->getType());
    TS_ASSERT_EQUALS(1, condition->getChildCount());

    // don't care about the condition.

    auto block = until->getChild(AstNodeType::Block);
    TS_ASSERT_EQUALS("", block->getData());
    TS_ASSERT_EQUALS(AstNodeType::Block, block->getType());
    TS_ASSERT_EQUALS(1, block->getChildCount());

    // don't care about the block contents
  }

  void testEachLoop() {
    auto ast = compiler.parse("each i in 1...3 {\n  //do something\n}");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto each = ast->getChild(0);
    TS_ASSERT_EQUALS("", each->getData());
    TS_ASSERT_EQUALS(AstNodeType::Each, each->getType());
    TS_ASSERT_EQUALS(2, each->getChildCount());

    auto iterator = each->getChild(0);
    TS_ASSERT_EQUALS("i", iterator->getData());
    TS_ASSERT_EQUALS(AstNodeType::Iterator, iterator->getType());
    TS_ASSERT_EQUALS(1, iterator->getChildCount());

    auto iterator_range = iterator->getChild(0);
    TS_ASSERT_EQUALS("", iterator_range->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExpressionRange, iterator_range->getType());
    TS_ASSERT_EQUALS(2, iterator_range->getChildCount());

    auto iterator_begin = iterator_range->getChild(0);
    TS_ASSERT_EQUALS("", iterator_begin->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExpressionRangeBegin, iterator_begin->getType());
    TS_ASSERT_EQUALS(1, iterator_begin->getChildCount());

    auto iterator_begin_int = iterator_begin->getChild(0);
    TS_ASSERT_EQUALS("1", iterator_begin_int->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, iterator_begin_int->getType());
    TS_ASSERT_EQUALS(0, iterator_begin_int->getChildCount());

    auto iterator_end = iterator_range->getChild(1);
    TS_ASSERT_EQUALS("", iterator_end->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExpressionRangeEnd, iterator_end->getType());
    TS_ASSERT_EQUALS(1, iterator_end->getChildCount());

    auto iterator_end_int = iterator_end->getChild(0);
    TS_ASSERT_EQUALS("3", iterator_end_int->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, iterator_end_int->getType());
    TS_ASSERT_EQUALS(0, iterator_end_int->getChildCount());

    auto block = each->getChild(1);
    TS_ASSERT_EQUALS("", block->getData());
    TS_ASSERT_EQUALS(AstNodeType::Block, block->getType());
    TS_ASSERT_EQUALS(1, block->getChildCount());

    // don't care about block contents.
  }

  void testExprWithParenthesis(void) {
    auto ast = compiler.parse("1 + (2 * 3) + 4");

    TS_ASSERT_EQUALS("root", ast->getData());
    TS_ASSERT_EQUALS(AstNodeType::Program, ast->getType());
    TS_ASSERT_EQUALS(1, ast->getChildCount());

    auto top_plus = ast->getChild(0);
    TS_ASSERT_EQUALS("+", top_plus->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, top_plus->getType());
    TS_ASSERT_EQUALS(2, top_plus->getChildCount());

    auto one = top_plus->getChild(0);
    TS_ASSERT_EQUALS("1", one->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, one->getType());
    TS_ASSERT_EQUALS(0, one->getChildCount());

    auto plus = top_plus->getChild(1);
    TS_ASSERT_EQUALS("+", plus->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorAdd, plus->getType());
    TS_ASSERT_EQUALS(2, plus->getChildCount());

    auto expr_group = plus->getChild(0);
    TS_ASSERT_EQUALS("", expr_group->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExprGroup, expr_group->getType());
    TS_ASSERT_EQUALS(1, expr_group->getChildCount());

    auto times = expr_group->getChild(0);
    TS_ASSERT_EQUALS("*", times->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorMultiply, times->getType());
    TS_ASSERT_EQUALS(2, times->getChildCount());

    auto two = times->getChild(0);
    TS_ASSERT_EQUALS("2", two->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, two->getType());
    TS_ASSERT_EQUALS(0, two->getChildCount());

    auto three = times->getChild(1);
    TS_ASSERT_EQUALS("3", three->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, three->getType());
    TS_ASSERT_EQUALS(0, three->getChildCount());

    auto four = plus->getChild(1);
    TS_ASSERT_EQUALS("4", four->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, four->getType());
    TS_ASSERT_EQUALS(0, four->getChildCount());
  }

  void testNotOperator() {
    auto ast = compiler.parse("!true || !false");

    auto or_operator = ast->getChild(0);
    TS_ASSERT_EQUALS("||", or_operator->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalDisjunction, or_operator->getType());
    TS_ASSERT_EQUALS(2, or_operator->getChildCount());

    auto left_not = or_operator->getChild(0);
    TS_ASSERT_EQUALS("", left_not->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalNegation, left_not->getType());
    TS_ASSERT_EQUALS(1, left_not->getChildCount());

    auto left_bool = left_not->getChild(0);
    TS_ASSERT_EQUALS("true", left_bool->getData());
    TS_ASSERT_EQUALS(AstNodeType::BooleanTrue, left_bool->getType());
    TS_ASSERT_EQUALS(0, left_bool->getChildCount());

    auto right_not = or_operator->getChild(1);
    TS_ASSERT_EQUALS("", right_not->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalNegation, right_not->getType());
    TS_ASSERT_EQUALS(1, right_not->getChildCount());

    auto right_bool = right_not->getChild(0);
    TS_ASSERT_EQUALS("false", right_bool->getData());
    TS_ASSERT_EQUALS(AstNodeType::BooleanFalse, right_bool->getType());
    TS_ASSERT_EQUALS(0, right_bool->getChildCount());
  }

  void testNotOperatorWithParenthesis() {
    auto ast = compiler.parse("!false || !(true && !false)");

    auto or_operator = ast->getChild(0);
    TS_ASSERT_EQUALS("||", or_operator->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalDisjunction, or_operator->getType());
    TS_ASSERT_EQUALS(2, or_operator->getChildCount());

    auto left_not = or_operator->getChild(0);
    TS_ASSERT_EQUALS("", left_not->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalNegation, left_not->getType());
    TS_ASSERT_EQUALS(1, left_not->getChildCount());

    auto left_bool = left_not->getChild(0);
    TS_ASSERT_EQUALS("false", left_bool->getData());
    TS_ASSERT_EQUALS(AstNodeType::BooleanFalse, left_bool->getType());
    TS_ASSERT_EQUALS(0, left_bool->getChildCount());

    auto right_not = or_operator->getChild(1);
    TS_ASSERT_EQUALS("", right_not->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalNegation, right_not->getType());
    TS_ASSERT_EQUALS(1, right_not->getChildCount());

    auto expr_group = right_not->getChild(0);
    TS_ASSERT_EQUALS("", expr_group->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExprGroup, expr_group->getType());
    TS_ASSERT_EQUALS(1, expr_group->getChildCount());

    auto and_operator = expr_group->getChild(0);
    TS_ASSERT_EQUALS("&&", and_operator->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalConjunction, and_operator->getType());
    TS_ASSERT_EQUALS(2, and_operator->getChildCount());

    auto and_operator_left = and_operator->getChild(0);
    TS_ASSERT_EQUALS("true", and_operator_left->getData());
    TS_ASSERT_EQUALS(AstNodeType::BooleanTrue, and_operator_left->getType());
    TS_ASSERT_EQUALS(0, and_operator_left->getChildCount());

    auto and_operator_right_not = and_operator->getChild(1);
    TS_ASSERT_EQUALS("", and_operator_right_not->getData());
    TS_ASSERT_EQUALS(AstNodeType::LogicalNegation, and_operator_right_not->getType());
    TS_ASSERT_EQUALS(1, and_operator_right_not->getChildCount());

    auto and_operator_right_bool = and_operator_right_not->getChild(0);
    TS_ASSERT_EQUALS("false", and_operator_right_bool->getData());
    TS_ASSERT_EQUALS(AstNodeType::BooleanFalse, and_operator_right_bool->getType());
    TS_ASSERT_EQUALS(0, and_operator_right_bool->getChildCount());
  }

  void testUnaryMinus() {
    auto ast = compiler.parse("-1 - (2 * -5)");

    auto minus = ast->getChild(0);
    TS_ASSERT_EQUALS("-", minus->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorSubtract, minus->getType());
    TS_ASSERT_EQUALS(2, minus->getChildCount());

    auto minus_left_unary_minus = minus->getChild(0);
    TS_ASSERT_EQUALS("", minus_left_unary_minus->getData());
    TS_ASSERT_EQUALS(AstNodeType::UnaryMinus, minus_left_unary_minus->getType());
    TS_ASSERT_EQUALS(1, minus_left_unary_minus->getChildCount());

    auto minus_left_integer = minus_left_unary_minus->getChild(0);
    TS_ASSERT_EQUALS("1", minus_left_integer->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, minus_left_integer->getType());
    TS_ASSERT_EQUALS(0, minus_left_integer->getChildCount());

    auto expr_group = minus->getChild(1);
    TS_ASSERT_EQUALS("", expr_group->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExprGroup, expr_group->getType());
    TS_ASSERT_EQUALS(1, expr_group->getChildCount());

    auto multiply = expr_group->getChild(0);
    TS_ASSERT_EQUALS("*", multiply->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorMultiply, multiply->getType());
    TS_ASSERT_EQUALS(2, multiply->getChildCount());

    auto multiply_left = multiply->getChild(0);
    TS_ASSERT_EQUALS("2", multiply_left->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, multiply_left->getType());
    TS_ASSERT_EQUALS(0, multiply_left->getChildCount());

    auto multiply_right_unary_minus = multiply->getChild(1);
    TS_ASSERT_EQUALS("", multiply_right_unary_minus->getData());
    TS_ASSERT_EQUALS(AstNodeType::UnaryMinus, multiply_right_unary_minus->getType());
    TS_ASSERT_EQUALS(1, multiply_right_unary_minus->getChildCount());

    auto multiply_right_integer = multiply_right_unary_minus->getChild(0);
    TS_ASSERT_EQUALS("5", multiply_right_integer->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, multiply_right_integer->getType());
    TS_ASSERT_EQUALS(0, multiply_right_integer->getChildCount());
  }

  void testUnaryMinusWithParenthesis() {
    auto ast = compiler.parse("2 * -(2 * -5)");

    auto multiply = ast->getChild(0);
    TS_ASSERT_EQUALS("*", multiply->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorMultiply, multiply->getType());
    TS_ASSERT_EQUALS(2, multiply->getChildCount());

    auto multiply_left = multiply->getChild(0);
    TS_ASSERT_EQUALS("2", multiply_left->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, multiply_left->getType());
    TS_ASSERT_EQUALS(0, multiply_left->getChildCount());

    auto unary_minus = multiply->getChild(1);
    TS_ASSERT_EQUALS("", unary_minus->getData());
    TS_ASSERT_EQUALS(AstNodeType::UnaryMinus, unary_minus->getType());
    TS_ASSERT_EQUALS(1, unary_minus->getChildCount());

    auto expr_group = unary_minus->getChild(0);
    TS_ASSERT_EQUALS("", expr_group->getData());
    TS_ASSERT_EQUALS(AstNodeType::ExprGroup, expr_group->getType());
    TS_ASSERT_EQUALS(1, expr_group->getChildCount());

    auto group_multiply = expr_group->getChild(0);
    TS_ASSERT_EQUALS("*", group_multiply->getData());
    TS_ASSERT_EQUALS(AstNodeType::BinaryOperatorMultiply, group_multiply->getType());
    TS_ASSERT_EQUALS(2, group_multiply->getChildCount());

    auto group_multiply_left = group_multiply->getChild(0);
    TS_ASSERT_EQUALS("2", group_multiply_left->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, group_multiply_left->getType());
    TS_ASSERT_EQUALS(0, group_multiply_left->getChildCount());

    auto group_unary_minus = group_multiply->getChild(1);
    TS_ASSERT_EQUALS("", group_unary_minus->getData());
    TS_ASSERT_EQUALS(AstNodeType::UnaryMinus, group_unary_minus->getType());
    TS_ASSERT_EQUALS(1, group_unary_minus->getChildCount());

    auto group_integer_right = group_unary_minus->getChild(0);
    TS_ASSERT_EQUALS("5", group_integer_right->getData());
    TS_ASSERT_EQUALS(AstNodeType::Integer, group_integer_right->getType());
    TS_ASSERT_EQUALS(0, group_integer_right->getChildCount());
  }

  void testDeclareType() {
    auto ast = compiler.parse("type MyType {\n  // comment\n}");

    auto type = ast->getChild(0);
    TS_ASSERT_EQUALS("MyType", type->getData());
    TS_ASSERT_EQUALS(AstNodeType::Type, type->getType());
    TS_ASSERT_EQUALS(1, type->getChildCount());

    auto block = type->getChild(0);
    TS_ASSERT_EQUALS("", block->getData());
    TS_ASSERT_EQUALS(AstNodeType::Block, block->getType());
    TS_ASSERT_EQUALS(1, block->getChildCount());

    // don't care about block contents.
  }

  void testPublicFunctionPrototype() {
    auto ast = compiler.parse("public function toString() -> string");

    auto func = ast->getChild(0);
    TS_ASSERT_EQUALS("toString", func->getData());
    TS_ASSERT_EQUALS(AstNodeType::Function, func->getType());
    TS_ASSERT_EQUALS(2, func->getChildCount());

    TS_ASSERT(func->hasChild(AstNodeType::TypeName));
    TS_ASSERT_EQUALS("string", func->getChild(AstNodeType::TypeName)->getData());

    TS_ASSERT(func->hasChild(AstNodeType::AccessSpecification));
    TS_ASSERT(func->getChild(AstNodeType::AccessSpecification)->hasChild(AstNodeType::AccessSpecifierPublic));
  }

  void testFunctionWithNameStructure() {
    auto ast = compiler.parse("function MyModule::MyType::toString(integer t) {\n// do something\n}");

    auto func = ast->getChild(0);
    TS_ASSERT_EQUALS("toString", func->getData());
    TS_ASSERT_EQUALS(AstNodeType::Function, func->getType());

    TS_ASSERT(func->hasChild(AstNodeType::FunctionParamDefinitions));

    TS_ASSERT(func->hasChild(AstNodeType::NameStructure));
    auto module_name_structure = func->getChild(AstNodeType::NameStructure);
    TS_ASSERT(module_name_structure->hasChild(AstNodeType::NameStructure));
    TS_ASSERT_EQUALS("MyModule", module_name_structure->getData());
    auto type_name_structure = module_name_structure->getChild(AstNodeType::NameStructure);
    TS_ASSERT_EQUALS("MyType", type_name_structure->getData());
  }
};
