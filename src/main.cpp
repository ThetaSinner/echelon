#include <iostream>
#include <map>
#include <list>

#include <echelon/parser/parser-data/parser-data-load.hpp>
#include <echelon/parser/stage2/nested-pattern-lookup.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>

std::string toString(bool b) {
  return b ? "true" : "false";
}

int main(int argc, char** args) {
  std::cout << "Clean main.\n";

  loadParserData();

  std::string binaryOperator = "binary_operator";

  std::string add = "add_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, add);
  std::string subtract = "subtract_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, subtract);
  std::string multiply = "multiply_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, multiply);
  std::string divide = "divide_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, divide);

  std::string expr = "expr";

  std::string function_call = "identifier paren_open [expr list_seperator]* [expr] paren_close [binary_operator expr]";
  NestedPatternLookup::getInstance() -> registerNested(expr, function_call);

  PatternTranslator patternTranslator;
  auto expressionPattern = patternTranslator.translate(expr);

  std::cout << std::endl;

  return 0;
}
