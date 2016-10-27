#include <iostream>
#include <echelon/parser/tokenizer.hpp>

#include <string>
#include <set>
#include <list>
#include <algorithm>

class EncodingQuery {
public:
  bool isEol(char c);
};

EncodingQuery* eq = new EncodingQuery();

bool EncodingQuery::isEol(char c) {
  return c == '\n';
}

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

  void setTerminateOnEol(bool b);
  bool isTerminateOnEol();
};

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

void TokenExpr::setTerminateOnEol(bool b) {
  terminateOnEol = b;
}

bool TokenExpr::isTerminateOnEol() {
  return terminateOnEol;
}

class MatchData {
private:
  int triggerLen = 0;
  int contentLen = 0;
  int terminateLen = 0;
  bool terminate = false;
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
    terminateLen = t;
  }
  bool isTerminate() {
    return terminate || terminateLen > 0;
  }
};

void matchTerminate(MatchData* matchData, TokenExpr* expr, std::string str, int pos) {
  int terminate_len = 0;
  for (int k = 0; pos + k < str.size(); k++) {
    if (expr -> isTerminateOnEol() && eq -> isEol(str.at(pos + k))) {
      matchData -> setTerminate(true);
      return;
    }

    if (expr -> isTerminateMatch(str.at(pos + k), k)) {
      terminate_len++;
    }
    else {
      break;
    }
  }

  matchData -> setTerminateLen(terminate_len);
}

void matchContent(MatchData* matchData, TokenExpr* expr, std::string str, int pos) {
  int content_len = 0;
  for (int i = 0; pos + i < str.size(); i++) {
    matchTerminate(matchData, expr, str, pos + content_len);
    if (matchData -> isTerminate()) {
      return;
    }

    if (expr -> isContentMatch(str.at(pos + i), i)) {
      content_len++;
    }

    // Need to update each loop because the function may exit before the loop does.
    matchData -> setContentLen(content_len);
  }
}

MatchData* match(TokenExpr* expr, std::string str, int pos) {
  MatchData* matchData = new MatchData();

  int trigger_len = 0;
  for (int k = 0; pos + k < str.size(); k++) {
    if (expr -> isTriggerMatch(str.at(pos + k), k)) {
      trigger_len++;
    }
    else {
      break;
    }
  }

  matchData -> setTriggerLen(trigger_len);

  if (trigger_len > 0) {
    matchContent(matchData, expr, str, pos + trigger_len);
  }

  return matchData;
}

int main(int argc, char** args) {
  TokenExpr mlc;
  mlc.setTriggerOrdered(true);
  mlc.addTriggerChar('/');
  mlc.addTriggerChar('*');

  mlc.setContentAny(true);

  mlc.setTerminateOrdered(true);
  mlc.addTerminateChar('*');
  mlc.addTerminateChar('/');

  TokenExpr slc;
  slc.setTriggerOrdered(true);
  slc.addTriggerChar('/');
  slc.addTriggerChar('/');

  slc.setContentAny(true);

  slc.setTerminateOnEol(true);

  TokenExpr identifier;
  identifier.addTriggerChars("a-z");
  identifier.addTriggerChars("A-Z");

  identifier.addContentChars("a-z");
  identifier.addContentChars("A-Z");
  identifier.addContentChars("0-9");
  identifier.addContentChar('_');

  TokenExpr assign;
  assign.setTriggerOrdered(true);
  assign.addTriggerChar('=');

  TokenExpr equality;
  equality.setTriggerOrdered(true);
  equality.addTriggerChar('=');
  equality.addTriggerChar('=');

  std::list<TokenExpr*> matchers2;
  matchers2.push_back(&mlc);
  matchers2.push_back(&slc);
  matchers2.push_back(&identifier);
  matchers2.push_back(&assign);
  matchers2.push_back(&equality);

  std::string str = "helloVariable thing bit stuff = == === /* hello */ /**/ // hey\n test words";
  int pos = 0;
  while (pos < str.size()) {
    if (str.at(pos) == ' ') {
      pos++;
      continue;
    }

    std::list<MatchData*> matches;
    for (auto& i : matchers2) {
      MatchData* matchData = match(i, str, pos);

      if (matchData -> isMatch()) {
        matches.push_back(matchData);
      }
    }

    if (matches.size()) {
      matches.sort([](MatchData* a, MatchData* b) {
        return a -> getLen() < b -> getLen();
      });

      int extract = matches.back() -> getLen();
      std::cout << "[" << str.substr(pos, extract) << "]\n";
      pos += extract;
    }
    else {
      // obviously this can't stay.
      std::cout << "unhandled char [" << str.at(pos) << "]\n";
      pos++;
    }
  }

  return 0;
}
