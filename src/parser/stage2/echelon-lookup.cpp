#include <echelon/parser/stage2/echelon-lookup.hpp>

#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/AstNodeType.hpp>
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

template<> std::string EchelonLookup::toString(TokenTypeEnum t) {
  switch(t) {
    case TokenTypeEnum::String:
      return "string";
    case TokenTypeEnum::Identifier:
      return "identifier";
    case TokenTypeEnum::Integer:
      return "integer";
    case TokenTypeEnum::BlockDelimO:
      return "block delim open";
    case TokenTypeEnum::BlockDelimC:
      return "block delim close";
    case TokenTypeEnum::Paren:
      return "paren";
    case TokenTypeEnum::ParenO:
      return "paren open";
    case TokenTypeEnum::ParenC:
      return "paren close";
    case TokenTypeEnum::OperatorAdd:
      return "add operator";
    case TokenTypeEnum::SqBracket:
      return "square bracket";
    case TokenTypeEnum::Assign:
      return "assign";
    case TokenTypeEnum::Equality:
      return "equality";
    case TokenTypeEnum::SingleLineComment:
      return "single line comment";
    case TokenTypeEnum::MultiLineComment:
      return "multi line comment";
    case TokenTypeEnum::StructureOperator:
      return "structure operator";
    case TokenTypeEnum::AndOperator:
      return "and operator";
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
    case AstNodeType::FunctionCall:
      return "function_call";
    case AstNodeType::Type:
      return "type";
    case AstNodeType::String:
      return "string";
    case AstNodeType::Integer:
      return "integer";
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
    default:
      #ifdef ECHELON_DEBUG
      throw std::runtime_error("Mising to string case for keyword.");
      #endif
      return "none";
  }
}
