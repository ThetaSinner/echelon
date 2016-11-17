#include <echelon/parser/stage2/enhanced-token.hpp>

#include <echelon/parser/stage2/echelon-lookup.hpp>

EnhancedToken::EnhancedToken(Token* t) {
  data = t -> getData();
  tokenType = t -> getTokenType();

  switch(tokenType) {
    case TokenTypeEnum::Identifier:
      keyword = EchelonLookup::getInstance() -> isKeyword(data);
      dataTypeKeyword = EchelonLookup::getInstance() -> isDataTypeKeyword(data);
      break;
    default:
      //std::cout << "Unhandled case in EnhancedToken constructor.\n";
      // do nothing.
      break;
  }
}

TokenTypeEnum EnhancedToken::getTokenType() {
  return tokenType;
}

std::string EnhancedToken::getData() {
  return data;
}

bool EnhancedToken::isKeyword() {
  return keyword;
}

bool EnhancedToken::isDataTypeKeyword() {
  return dataTypeKeyword;
}
