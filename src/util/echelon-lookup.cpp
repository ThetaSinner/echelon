#include <echelon/util/echelon-lookup.hpp>

#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

#include <stdexcept>

EchelonLookup* EchelonLookup::self = nullptr;

EchelonLookup* EchelonLookup::getInstance() {
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

template<> std::string EchelonLookup::toString(TokenType t) {
  switch(t) {
    case TokenType::String:
      return "string";
    case TokenType::Identifier:
      return "identifier";
    case TokenType::Integer:
      return "integer";
    case TokenType::Float:
      return "float";
    case TokenType::BlockDelimO:
      return "block delim open";
    case TokenType::BlockDelimC:
      return "block delim close";
    case TokenType::Paren:
      return "paren";
    case TokenType::ParenO:
      return "paren open";
    case TokenType::ParenC:
      return "paren close";
    case TokenType::OperatorAdd:
      return "add operator";
    case TokenType::OperatorSubtract:
      return "subtract operator";
    case TokenType::OperatorMultiply:
      return "multiply operator";
    case TokenType::OperatorDivide:
      return "divide operator";
    case TokenType::SqBracket:
      return "square bracket";
    case TokenType::Assign:
      return "assign";
    case TokenType::Equality:
      return "equality";
    case TokenType::SingleLineComment:
      return "single line comment";
    case TokenType::MultiLineComment:
      return "multi line comment";
    case TokenType::StructureOperator:
      return "structure operator";
    case TokenType::CommaOperator:
      return "comma operator";
    case TokenType::AndOperator:
      return "and operator";
    case TokenType::OrOperator:
      return "or operator";
  }

  throw std::runtime_error("Missing to string case for token type enum.");
}

template<> std::string EchelonLookup::toString(bool b) {
  return b ? "true" : "false";
}

template<> std::string EchelonLookup::toString(AstNodeType t) {
  switch(t) {
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
    case AstNodeType::BinaryOperator:
      return "binary_operator";
    case AstNodeType::BooleanBinaryOperator:
      return "boolean_binary_operator";
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
    case AstNodeType::Float:
      return "float";
    case AstNodeType::Boolean:
      return "boolean";
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
  }

  throw std::runtime_error("Mising to string case for ast node type.");
}

template<> std::string EchelonLookup::toString(Keyword t) {
  switch(t) {
    case Keyword::Package:
      return "package";
    case Keyword::Enum:
      return "enum";
    case Keyword::For:
      return "for";
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
  }

  throw std::runtime_error("Missing to string case for keyword.");
}

template<> std::string EchelonLookup::toString(EnhancedAstNodeType t) {
  switch (t) {
    case EnhancedAstNodeType::None:
      return "none";
    case EnhancedAstNodeType::Program:
      return "program";
    case EnhancedAstNodeType::Type:
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

template<> std::string EchelonLookup::toString(EnhancedAstNodeSubType t) {
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
