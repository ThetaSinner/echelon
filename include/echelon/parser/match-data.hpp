#ifndef MATCH_DATA_HPP
#define MATCH_DATA_HPP

#include <echelon/parser/token-type-enum.hpp>

class MatchData {
private:
  int triggerLen = 0;
  int contentLen = 0;
  int terminateLen = 0;
  bool terminate = false;
  TokenTypeEnum tokenTypeEnum;
public:
  void setTriggerLen(int len) {
    triggerLen = len;
  }
  void setContentLen(int len) {
    contentLen = len;
  }
  void setTerminateLen(int len) {
    terminateLen = len;
  }

  int getLen() {
    return triggerLen + contentLen + terminateLen;
  }

  bool isMatch() {
    return getLen() > 0;
  }

  void setTerminate(bool t) {
    terminate = t;
  }
  bool isTerminate();

  void setTokenTypeEnum(TokenTypeEnum tokenTypeEnum);
  TokenTypeEnum getTokenTypeEnum();
};

#endif
