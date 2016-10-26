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

int matchTerminate(std::list<TokenExpr*> matchers, std::string str, int pos, int offset) {
  if (pos + offset >= str.size()) {
    return offset;
  }

  int term_len = 0;
  for (int k = 0; pos + k < str.size(); k++) {
    bool anyMatched = false;
    for (auto& i : matchers) {
      if (i -> isTerminateOnEol() && eq -> isEol(str.at(pos + k))) {
        return 0;
      }

      if (i -> isTerminateMatch(str.at(pos + k), k)) {
        anyMatched = true;
        break;
      }
    }

    if (anyMatched) {
      term_len = k + 1;
    }
    else {
      break;
    }
  }

  return term_len == 0 ? -1 : term_len;
}

int matchContent(std::list<TokenExpr*> matchers, std::string str, int pos, int offset) {
  if (pos + offset >= str.size()) {
    return offset;
  }

  int mt = matchTerminate(matchers, str, pos + offset, 0);
  if (mt != -1) {
    return offset + mt;
  }

  std::list<TokenExpr*> rest_matchers;
  for (auto& i : matchers) {
    //std::cout << "?[" << str.at(pos + offset) << "] ";
    if (i -> isContentMatch(str.at(pos + offset), offset)) {
      rest_matchers.push_back(i);
    }
  }

  if (rest_matchers.size()) {
    return matchContent(rest_matchers, str, pos, offset + 1);
  }
  else {
    return offset;
  }
}

int match(std::list<TokenExpr*> matchers, std::string str, int pos) {
  std::list<TokenExpr*> first_matchers;
  int trigger_len = 0;
  for (int k = 0; pos + k < str.size(); k++) {
    std::list<TokenExpr*> first_matchers_sub;
    for (auto& i : matchers) {
      if (i -> isTriggerMatch(str.at(pos + k), k)) {
        first_matchers_sub.push_back(i);
      }
      // todo make use of maybe.
    }

    if (first_matchers_sub.size()) {
      first_matchers.clear();
      first_matchers.insert(first_matchers.end(), first_matchers_sub.begin(), first_matchers_sub.end());
      trigger_len = k + 1;
    }
    else {
      break;
    }
  }

  if (first_matchers.size()) {
    return matchContent(first_matchers, str, pos + trigger_len, 0) + trigger_len;
  }
  else {
    return -1;
  }
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

    int extract = match(matchers2, str, pos);

    if (extract != -1) {
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
