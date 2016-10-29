#ifndef TOKEN_EXPR_HPP
#define TOKEN_EXPR_HPP

#include <string>
#include <vector>
#include <set>

#include <echelon/parser/token-expr-info.hpp>

class TokenExpr {
private:
  bool triggerOrdered = false;
  std::vector<char> triggerChars;

  bool contentOrdered = false;
  std::vector<char> contentChars;
  bool contentAny = false;

  bool terminateOrdered = false;
  std::vector<char> terminateChars;
  bool terminateOnEol = false;

  std::set<char> expandExpr(std::string expr);

  TokenExprInfo tokenExprInfo;
public:
  void setTriggerOrdered(bool ordered) {
    triggerOrdered = ordered;
  }
  bool isTriggerOrdered() {
    return triggerOrdered;
  }
  void addTriggerChar(char c);
  void addTriggerChars(std::string expr);
  bool isTriggerMatch(char c, int index);

  bool isCompleteTrigger(int currentLen);

  void setContentOrdered(bool ordered) {
    contentOrdered = ordered;
  }
  bool isContentOrdered() {
    return contentOrdered;
  }
  void addContentChar(char c);
  void addContentChars(std::string expr);
  void setContentAny(bool b);
  bool isContentMatch(char c, int index);

  void setTerminateOrdered(bool ordered) {
    terminateOrdered = ordered;
  }
  bool isTerminateOrdered() {
    return terminateOrdered;
  }
  void addTerminateChar(char c);
  void addTerminateChars(std::string expr);
  bool isTerminateMatch(char c, int index);

  bool isCompleteTerminate(int currentLen);

  void setTerminateOnEol(bool b);
  bool isTerminateOnEol();

  TokenExprInfo* getTokenExprInfo() {
    return &tokenExprInfo;
  }
};

#endif
