#include <echelon/util/echelon-lookup.hpp>

#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

#include <stdexcept>
#include <echelon/parser/stage1/character-pattern-group-type-enum.hpp>

EchelonLookup *EchelonLookup::self = nullptr;

EchelonLookup *EchelonLookup::getInstance() {
  if (self == nullptr) {
    self = new EchelonLookup();
  }

  return self;
}

void EchelonLookup::addDataTypeKeyword(std::string dataTypeKeyword) {
  dataTypeKeywordSet.insert(dataTypeKeyword);

  keywordSet.insert(dataTypeKeyword);
}

bool EchelonLookup::isDataTypeKeyword(std::string str) {
  return dataTypeKeywordSet.find(str) != dataTypeKeywordSet.end();
}

void EchelonLookup::addKeyword(std::string keyword) {
  keywordSet.insert(keyword);
}

bool EchelonLookup::isKeyword(std::string str) {
  return keywordSet.find(str) != keywordSet.end();
}

template<>
std::string EchelonLookup::toString(CharacterPatternGroupType cpgt) {
  switch (cpgt) {
    case CharacterPatternGroupType::Sequence:
      return "sequence";
    case CharacterPatternGroupType::Union:
      return "union";
  }

  throw std::runtime_error("Missing to string case for character pattern group type.");
}

template<>
std::string EchelonLookup::toString(TokenType t) {
  switch (t) {
    case TokenType::Identifier:
      return "identifier";
    case TokenType::Decimal:
      return "decimal";
    case TokenType::Integer:
      return "integer";
    case TokenType::String:
      return "string";
    case TokenType::SingleLineComment:
      return "single line comment";
    case TokenType::MultiLineComment:
      return "multi line comment";
    case TokenType::LogicalConjunction:
      return "logical conjunction";
    case TokenType::LogicalDisjunction:
      return "logical disjunction";
    case TokenType::LogicalNegation:
      return "logical negation";
    case TokenType::LogicalEquality:
      return "logical equality";
    case TokenType::BlockDelimiterOpen:
      return "block delimiter open";
    case TokenType::BlockDelimiterClose:
      return "block delimiter close";
    case TokenType::ParenthesisOpen:
      return "parenthesis open";
    case TokenType::ParenthesisClose:
      return "parenthesis close";
    case TokenType::Assignment:
      return "assignment";
    case TokenType::AddOperator:
      return "add operator";
    case TokenType::SubtractOperator:
      return "subtract operator";
    case TokenType::MultiplyOperator:
      return "multiply operator";
    case TokenType::DivideOperator:
      return "divide operator";
    case TokenType::Comma:
      return "comma operator";
    case TokenType::StructureOperator:
      return "structure operator";
    case TokenType::ForwardArrowOperator:
      return "forward arrow operator";
    case TokenType::EllipsisOperator:
      return "ellipsis operator";

  }

  throw std::runtime_error("Missing to string case for token type enum.");
}

template<>
std::string EchelonLookup::toString(bool b) {
  return b ? "true" : "false";
}

template<>
std::string EchelonLookup::toString(AstNodeType t) {
  switch (t) {
    case AstNodeType::Program:
      return "program";
    case AstNodeType::Block:
      return "block";
    case AstNodeType::Variable:
      return "variable";
    case AstNodeType::ExplicitType:
      return "explicit type";
    case AstNodeType::Package:
      return "package";
    case AstNodeType::Enum:
      return "enum";
    case AstNodeType::EnumConstant:
      return "enum constant";
    case AstNodeType::EnumConstants:
      return "enum constants";
    case AstNodeType::Module:
      return "module";
    case AstNodeType::BinaryOperatorAdd:
      return "binary operator add";
    case AstNodeType::BinaryOperatorSubtract:
      return "binary operator subtract";
    case AstNodeType::BinaryOperatorMultiply:
      return "binary operator multiply";
    case AstNodeType::BinaryOperatorDivide:
      return "binary operator divide";
    case AstNodeType::LogicalDisjunction:
      return "logical disjunction";
    case AstNodeType::FunctionCall:
      return "function_call";
    case AstNodeType::FunctionCallParams:
      return "function call params";
    case AstNodeType::FunctionCallParam:
      return "function call param";
    case AstNodeType::Function:
      return "function";
    case AstNodeType::FunctionParamDefinitions:
      return "function param defs";
    case AstNodeType::FunctionParamDefinition:
      return "function param def";
    case AstNodeType::Type:
      return "type";
    case AstNodeType::String:
      return "string";
    case AstNodeType::Integer:
      return "integer";
    case AstNodeType::Decimal:
      return "decimal";
    case AstNodeType::BooleanTrue:
      return "boolean true";
    case AstNodeType::BooleanFalse:
      return "boolean false";
    case AstNodeType::EqualityOperator:
      return "equality_operator";
    case AstNodeType::If:
      return "if";
    case AstNodeType::ElseIf:
      return "else if";
    case AstNodeType::Else:
      return "else";
    case AstNodeType::Branches:
      return "branches";
    case AstNodeType::SingleLineComment:
      return "single line comment";
    case AstNodeType::MultiLineComment:
      return "multi line comment";
    case AstNodeType::Until:
      return "until";
    case AstNodeType::While:
      return "while";
    case AstNodeType::Each:
      return "each";
    case AstNodeType::Condition:
      return "condition";
    case AstNodeType::Behaviour:
      return "behaviour";
    case AstNodeType::ExpressionRange:
      return "expression range";
    case AstNodeType::ExpressionRangeBegin:
      return "expression range begin";
    case AstNodeType::ExpressionRangeEnd:
      return "expression range end";
    case AstNodeType::Iterator:
      return "iterator";
    case AstNodeType::ExprGroup:
      return "expr group";
    case AstNodeType::LogicalNegation:
      return "logical negation";
    case AstNodeType::UnaryMinus:
      return "unary minus";
    case AstNodeType::TypeName:
      return "type name";
    case AstNodeType::LogicalConjunction:
      return "logical conjunction";
  }

  throw std::runtime_error("Mising to string case for ast node type.");
}

template<>
std::string EchelonLookup::toString(Keyword t) {
  switch (t) {
    case Keyword::Package:
      return "package";
    case Keyword::Enum:
      return "enum";
    case Keyword::For:
      return "for";
    case Keyword::Until:
      return "until";
    case Keyword::While:
      return "while";
    case Keyword::Each:
      return "each";
    case Keyword::Module:
      return "module";
    case Keyword::If:
      return "if";
    case Keyword::Else:
      return "else";
    case Keyword::Integer:
      return "integer";
    case Keyword::String:
      return "string";
    case Keyword::True:
      return "true";
    case Keyword::False:
      return "false";
    case Keyword::Behaviour:
      return "behaviour";
    case Keyword::Function:
      return "function";
    case Keyword::In:
      return "in";
    case Keyword::Type:
      return "type";
  }

  throw std::runtime_error("Missing to string case for keyword.");
}

template<>
std::string EchelonLookup::toString(EnhancedAstNodeType t) {
  switch (t) {
    case EnhancedAstNodeType::None:
      return "none";
    case EnhancedAstNodeType::Program:
      return "program";
    case EnhancedAstNodeType::TypeName:
      return "type";
    case EnhancedAstNodeType::Variable:
      return "variable";
    case EnhancedAstNodeType::PrimitiveValue:
      return "primitive value";
    case EnhancedAstNodeType::Function:
      return "function";
    case EnhancedAstNodeType::FunctionParamDefinition:
      return "function param definition";
    case EnhancedAstNodeType::FunctionParamDefinitions:
      return "function param definitions";
  }

  throw std::runtime_error("Missing to string case for enhanced ast node type.");
}

template<>
std::string EchelonLookup::toString(EnhancedAstNodeSubType t) {
  switch (t) {
    case EnhancedAstNodeSubType::Assign:
      return "assign";
    case EnhancedAstNodeSubType::Declaration:
      return "declaration";
    case EnhancedAstNodeSubType::Integer:
      return "integer";
    case EnhancedAstNodeSubType::IntegerType:
      return "integer type";
    case EnhancedAstNodeSubType::Unspecified:
      return "unspecified";
  }

  throw std::runtime_error("Missing to string case for enhanced ast node sub type.");
}
