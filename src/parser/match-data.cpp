#include <echelon/parser/match-data.hpp>

bool MatchData::isTerminate() {
  return terminate || terminateLen > 0;
}

void MatchData::setTokenTypeEnum(TokenTypeEnum tokenTypeEnum) {
  this -> tokenTypeEnum = tokenTypeEnum;
}

TokenTypeEnum MatchData::getTokenTypeEnum() {
  return tokenTypeEnum;
}
