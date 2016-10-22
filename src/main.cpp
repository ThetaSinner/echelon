#include <iostream>
#include <echelon/parser/tokenizer.hpp>

#include <string>
#include <set>
#include <list>

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

  return 0;
}
