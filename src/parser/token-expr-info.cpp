#include <echelon/parser/token-expr-info.hpp>

void TokenExprInfo::setFriendlyName(std::string friendlyName) {
  this -> friendlyName = friendlyName;
}

std::string TokenExprInfo::getFriendlyName() {
  return friendlyName;
}

void TokenExprInfo::setTokenType(TokenTypeEnum tokenType) {
  this -> tokenType = tokenType;
}

TokenTypeEnum TokenExprInfo::getTokenType() {
  return tokenType;
}
