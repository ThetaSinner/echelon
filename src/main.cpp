#include <iostream>
#include <echelon/parser/tokenizer.hpp>

#include <string>
#include <set>
#include <list>
#include <algorithm>

enum class Bool {
  Yes,
  No,
  Maybe
};

std::string b(Bool input) {
  if (input == Bool::Yes) {
    return "yes";
  }
  else if (input == Bool::No) {
    return "no";
  }
  else {
    return "maybe";
  }
}

class TokenExpr2 {
private:
  bool triggerOrdered = false;
  std::vector<char> triggerChars;

  bool contentOrdered = false;
  std::vector<char> contentChars;

  bool terminateOrdered = false;
  std::vector<char> terminateChars;

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
  Bool isTriggerMatch(char c, int index);

  void setContentOrdered(bool ordered) {
    contentOrdered = ordered;
  }
  bool isContentOrdered() {
    return contentOrdered;
  }
  void addContentChar(char c);
  void addContentChars(std::string expr);
  Bool isContentMatch(char c, int index);

  void setTerminateOrdered(bool ordered) {
    terminateOrdered = ordered;
  }
  bool isTerminateOrdered() {
    return terminateOrdered;
  }
  void addTerminateChar(char c);
  void addTerminateChars(std::string expr);
  Bool isTerminateMatch(char c, int index);
};

std::set<char> TokenExpr2::expandExpr(std::string expr) {
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

void TokenExpr2::addTriggerChar(char c) {
  triggerChars.push_back(c);
}

void TokenExpr2::addTriggerChars(std::string expr) {
  auto expanded = expandExpr(expr);
  triggerChars.insert(triggerChars.end(), expanded.begin(), expanded.end());
}

Bool TokenExpr2::isTriggerMatch(char c, int index) {
 if (triggerOrdered) {
   if (index < triggerChars.size()) {
     bool is = triggerChars.at(index) == c;
     if (index + 1 == triggerChars.size()) {
       return is ? Bool::Yes  : Bool::No;
     }
     else {
       return is ? Bool::Maybe : Bool::No;
     }
   }
   else {
     return Bool::No;
   }
 }
 else {
   bool r = std::find(triggerChars.begin(), triggerChars.end(), c) != triggerChars.end();
   return r ? Bool::Yes : Bool::No;
 }
}

void TokenExpr2::addContentChar(char c) {
  contentChars.push_back(c);
}

void TokenExpr2::addContentChars(std::string expr) {
  auto expanded = expandExpr(expr);
  contentChars.insert(contentChars.end(), expanded.begin(), expanded.end());
}

Bool TokenExpr2::isContentMatch(char c, int index) {
  if (contentOrdered) {
    if (index < contentChars.size()) {
      bool r = contentChars.at(index) == c;
      return r ? Bool::Yes : Bool::No;
    }
    else {
      return Bool::No;
    }
  }
  else {
    bool r = std::find(contentChars.begin(), contentChars.end(), c) != contentChars.end();
    return r ? Bool::Yes : Bool::No;
  }
}

void TokenExpr2::addTerminateChar(char c) {
  terminateChars.push_back(c);
}

void TokenExpr2::addTerminateChars(std::string expr) {
  auto expanded = expandExpr(expr);
  terminateChars.insert(terminateChars.end(), expanded.begin(), expanded.end());
}

Bool TokenExpr2::isTerminateMatch(char c, int index) {
 if (terminateOrdered) {
   if (index < terminateChars.size()) {
     bool is = terminateChars.at(index) == c;
     if (index + 1 == terminateChars.size()) {
       return is ? Bool::Yes  : Bool::No;
     }
     else {
       return is ? Bool::Maybe : Bool::No;
     }
   }
   else {
     return Bool::No;
   }
 }
 else {
   bool r = std::find(terminateChars.begin(), terminateChars.end(), c) != terminateChars.end();
   return r ? Bool::Yes : Bool::No;
 }
}

class TokenExpr {
private:
  std::set<char> triggerChars;
protected:
  std::set<char> expandExpr(std::string expr);
public:
  void addTriggerChar(char c);
  void addTriggerChars(std::string expr);

  bool isAllowedFirstChar(char c);
  virtual bool isAllowedChar(char, int index) = 0;
};

void TokenExpr::addTriggerChar(char c) {
  triggerChars.insert(c);
}

void TokenExpr::addTriggerChars(std::string expr) {
  auto expanded = expandExpr(expr);
  triggerChars.insert(expanded.begin(), expanded.end());
}

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

bool TokenExpr::isAllowedFirstChar(char c) {
  if (triggerChars.find(c) != triggerChars.end()) {
    return true;
  }

  return false;
}

class TokenUnordExpr : public TokenExpr {
private:
  std::set<char> restChars;

public:
  void addRestChar(char c);
  void addRestChars(std::string expr);

  bool isAllowedChar(char, int index);
};

void TokenUnordExpr::addRestChar(char c) {
  restChars.insert(c);
}

void TokenUnordExpr::addRestChars(std::string expr) {
  auto expanded = expandExpr(expr);
  restChars.insert(expanded.begin(), expanded.end());
}

bool TokenUnordExpr::isAllowedChar(char c, int index) {
  // we don't care about order.
  return restChars.find(c) != restChars.end();
}

class TokenSeqExpr : public TokenExpr {
private:
  std::vector<char> restSeq;

public:
  void addRestSeq(std::string seq);

  bool isAllowedChar(char, int index);
};

void TokenSeqExpr::addRestSeq(std::string seq) {
  for (int i = 0; i < seq.size(); i++) {
    restSeq.push_back(seq.at(i));
  }
}

bool TokenSeqExpr::isAllowedChar(char c, int index) {
  return index < restSeq.size() && restSeq.at(index) == c;
}

int matchRest(std::list<TokenExpr*> matchers, std::string str, int pos, int offset) {
  if (pos + offset >= str.size()) {
    return offset;
  }

  std::list<TokenExpr*> rest_matchers;
  for (auto& i : matchers) {
    std::cout << "?[" << str.at(pos + offset) << "] ";
    if (i -> isAllowedChar(str.at(pos + offset), offset)) {
      rest_matchers.push_back(i);
    }
  }

  if (rest_matchers.size()) {
    return matchRest(rest_matchers, str, pos, offset + 1);
  }
  else {
    return offset;
  }
}

int match(std::list<TokenExpr*> matchers, std::string str, int pos) {
  std::list<TokenExpr*> first_matchers;
  for (auto& i : matchers) {
    if (i -> isAllowedFirstChar(str.at(pos))) {
      first_matchers.push_back(i);
    }
  }

  if (first_matchers.size()) {
    return matchRest(first_matchers, str, pos + 1, 0);
  }
  else {
    return -1;
  }
}

int main(int argc, char** args) {
  std::cout << "Happy dappy" << std::endl;

  Tokenizer t;
  t.tokenize("hello world");
  t.tokenize("hello_world");
  t.tokenize("12 3he4llo5 67 8wo9rld10 11");

  TokenUnordExpr* te = new TokenUnordExpr();
  te->addTriggerChar('=');

  if (te->isAllowedFirstChar('=')) {
    std::cout << "good\n";
  }
  else {
    std::cout << "bad\n";
  }

  if (te->isAllowedFirstChar('@')) {
    std::cout << "bad\n";
  }
  else {
    std::cout << "good\n";
  }

  TokenUnordExpr* te2 = new TokenUnordExpr();
  te2->addTriggerChars("a-z");

  if (te2->isAllowedFirstChar('k')) {
    std::cout << "good\n";
  }
  else {
    std::cout << "bad\n";
  }

  if (te2->isAllowedFirstChar('A')) {
    std::cout << "bad\n";
  }
  else {
    std::cout << "good\n";
  }

  TokenUnordExpr* tue = new TokenUnordExpr();
  tue -> addRestChar('_');

  if (tue -> isAllowedChar('_', 1)) {
    std::cout << "good\n";
  }
  else {
    std::cout << "bad\n";
  }

  TokenSeqExpr* tse = new TokenSeqExpr();
  tse -> addRestSeq("=="); // tripple equals, incl the trigger char.

  if (tse -> isAllowedChar('=', 0)) {
    std::cout << "good\n";
  }
  else {
    std::cout << "bad\n";
  }

  if (tse -> isAllowedChar('=', 1)) {
    std::cout << "good\n";
  }
  else {
    std::cout << "bad\n";
  }

  if (tse -> isAllowedChar('=', 2)) {
    std::cout << "bad\n";
  }
  else {
    std::cout << "good\n";
  }

  TokenUnordExpr* identifier = new TokenUnordExpr();
  identifier -> addTriggerChars("a-z");
  identifier -> addTriggerChars("A-Z");

  identifier -> addRestChars("a-z");
  identifier -> addRestChars("A-Z");
  identifier -> addRestChars("0-9");
  identifier -> addRestChar('_');

  TokenSeqExpr* equality = new TokenSeqExpr();
  equality -> addTriggerChar('=');
  equality -> addRestSeq("=");

  TokenExpr* assign = new TokenSeqExpr();
  assign -> addTriggerChar('=');

  std::list<TokenExpr*> matchers;
  matchers.push_back(identifier);
  matchers.push_back(equality);
  matchers.push_back(assign);

  std::string str = "helloVariable thing bit stuff = == ===";
  int pos = 0;
  while (pos < str.size()) {
    if (str.at(pos) == ' ') {
      pos++;
      continue;
    }

    int extract = match(matchers, str, pos);

    if (extract != -1) {
      std::cout << str.substr(pos, extract + 1) << "\n";
      pos += extract + 1;
    }
    else {
      std::cout << "unhandled char\n";
      pos += 1;
    }
  }

  TokenExpr2 tev2;
  tev2.setTriggerOrdered(true);
  tev2.addTriggerChar('<');
  tev2.addTriggerChar('=');

  std::cout << "[Maybe] " << b(tev2.isTriggerMatch('<', 0)) << "\n";
  std::cout << "[No] " << b(tev2.isTriggerMatch('=', 0)) << "\n";
  std::cout << "[Yes] " << b(tev2.isTriggerMatch('=', 1)) << "\n";
  std::cout << "[No] " << b(tev2.isTriggerMatch('t', 3)) << "\n";

  TokenExpr2 mlc;
  mlc.setTriggerOrdered(true);
  mlc.addTriggerChar('/');
  mlc.addTriggerChar('*');

  std::cout << "[Maybe] " << b(mlc.isTriggerMatch('/', 0)) << "\n";
  std::cout << "[Yes] " << b(mlc.isTriggerMatch('*', 1)) << "\n";

  return 0;
}
