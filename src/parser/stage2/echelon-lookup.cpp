#include <echelon/parser/stage2/echelon-lookup.hpp>

#include <echelon/parser/token-type-enum.hpp>
#include <echelon/ast/AstNodeType.hpp>
#include <echelon/parser/keyword-enum.hpp>

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
    case TokenTypeEnum::Identifier:
      return "identifier";
    case TokenTypeEnum::BlockDelimO:
      return "block delim open";
    case TokenTypeEnum::BlockDelimC:
      return "block delim close";
    case TokenTypeEnum::Assign:
      return "assign";
    case TokenTypeEnum::OperatorAdd:
      return "add operator";
    case TokenTypeEnum::ParenO:
      return "paren open";
    case TokenTypeEnum::ParenC:
      return "paren close";
    default:
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
    case AstNodeType::Package:
      return "package";
    case AstNodeType::Module:
      return "module";
    case AstNodeType::Block:
      return "block";
    case AstNodeType::Variable:
      return "variable";
    case AstNodeType::BinaryOperator:
      return "binary operator";
    case AstNodeType::FunctionCall:
      return "function call";
    case AstNodeType::Type:
      return "type";
    default:
      return "none";
  }
}

template<> std::string EchelonLookup::toString(Keyword t) {
  switch(t) {
    case Keyword::Package:
      return "package";
    case Keyword::Module:
      return "module";
    default:
      return "none";
  }
}
