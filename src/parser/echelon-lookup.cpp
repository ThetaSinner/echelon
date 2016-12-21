#include <echelon/parser/echelon-lookup.hpp>

#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>

#ifdef ECHELON_DEBUG
#include <stdexcept>
#include <iostream>
#endif

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
    case TokenType::AndOperator:
      return "and operator";
    case TokenType::OrOperator:
      return "or operator";
    default:
      #ifdef ECHELON_DEBUG
      throw std::runtime_error("Missing to string case for token type enum.");
      #endif
      return "none";
  }
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
    case AstNodeType::Module:
      return "module";
    case AstNodeType::BinaryOperator:
      return "binary_operator";
    case AstNodeType::BooleanBinaryOperator:
      return "boolean binary operator";
    case AstNodeType::FunctionCall:
      return "function_call";
    case AstNodeType::Type:
      return "type";
    case AstNodeType::String:
      return "string";
    case AstNodeType::Integer:
      return "integer";
    case AstNodeType::Boolean:
      return "boolean";
    case AstNodeType::EqualityOperator:
      return "equality_operator";
    case AstNodeType::If:
      return "if";
    case AstNodeType::Else:
      return "else";
    case AstNodeType::Branches:
      return "branches";
    default:
      #ifdef ECHELON_DEBUG
      throw std::runtime_error("Mising to string case for ast node type.");
      #endif
      return "none";
  }
}

template<> std::string EchelonLookup::toString(Keyword t) {
  switch(t) {
    case Keyword::Package:
      return "package";
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
    default:
      #ifdef ECHELON_DEBUG
      throw std::runtime_error("Mising to string case for keyword.");
      #endif
      return "none";
  }
}
