#include <echelon/parser/token-expr.hpp>

#include <algorithm>

std::set<char> TokenExpr::expandExpr(std::string expr) {
  int expandIndex = expr.find("-");
  if (expandIndex != -1) {
    char first = expr.at(0);
    char last = expr.at(2);

    std::set<char> out;
    for (int i = (int) first; i < (int) last; i++) {
      out.insert((char) i);
    }

    return out;
  }
  else {
    std::set<char> out;
    for (int i = 0; i < expr.size(); i++) {
      out.insert(expr.at(i));
    }

    return out;
  }
}

void TokenExpr::addTriggerChar(char c) {
  triggerChars.push_back(c);
}

void TokenExpr::addTriggerChars(std::string expr) {
  auto expanded = expandExpr(expr);
  triggerChars.insert(triggerChars.end(), expanded.begin(), expanded.end());
}

bool TokenExpr::isTriggerMatch(char c, int index) {
 if (triggerOrdered) {
   if (index < triggerChars.size()) {
     return triggerChars.at(index) == c;
   }
   else {
     return false;
   }
 }
 else {
   return std::find(triggerChars.begin(), triggerChars.end(), c) != triggerChars.end();
 }
}

bool TokenExpr::isCompleteTrigger(int currentLen) {
  if (isTriggerOrdered() && currentLen < triggerChars.size()) {
    return false;
  }

  return true;
}

void TokenExpr::addContentChar(char c) {
  contentChars.push_back(c);
}

void TokenExpr::addContentChars(std::string expr) {
  auto expanded = expandExpr(expr);
  contentChars.insert(contentChars.end(), expanded.begin(), expanded.end());
}

void TokenExpr::setContentAny(bool b) {
  contentAny = b;
}

bool TokenExpr::isContentMatch(char c, int index) {
  if (contentAny) {
    return true;
  }

  if (contentOrdered) {
    if (index < contentChars.size()) {
      return contentChars.at(index) == c;
    }
    else {
      return false;
    }
  }
  else {
    return std::find(contentChars.begin(), contentChars.end(), c) != contentChars.end();
  }
}

void TokenExpr::addTerminateChar(char c) {
  terminateChars.push_back(c);
}

void TokenExpr::addTerminateChars(std::string expr) {
  auto expanded = expandExpr(expr);
  terminateChars.insert(terminateChars.end(), expanded.begin(), expanded.end());
}

bool TokenExpr::isTerminateMatch(char c, int index) {
 if (terminateOrdered) {
   if (index < terminateChars.size()) {
     return terminateChars.at(index) == c;
   }
   else {
     return false;
   }
 }
 else {
   return std::find(terminateChars.begin(), terminateChars.end(), c) != terminateChars.end();
 }
}

bool TokenExpr::isCompleteTerminate(int currentLen) {
  if (isTerminateOrdered() && currentLen < terminateChars.size()) {
    return false;
  }

  return true;
}

void TokenExpr::setTerminateOnEol(bool b) {
  terminateOnEol = b;
}

bool TokenExpr::isTerminateOnEol() {
  return terminateOnEol;
}
