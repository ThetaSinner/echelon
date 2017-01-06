#include <echelon/parser/stage2/enhanced-token.hpp>

#include <echelon/util/echelon-lookup.hpp>

EnhancedToken::EnhancedToken(Token *t) {
  data = t->getData();
  tokenType = t->getTokenType();

  switch (tokenType) {
    case TokenType::Identifier:
      keyword = EchelonLookup::getInstance()->isKeyword(data);
      dataTypeKeyword = EchelonLookup::getInstance()->isDataTypeKeyword(data);
      break;
    default:
      // do nothing.
      break;
  }
}

TokenType EnhancedToken::getTokenType() {
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
