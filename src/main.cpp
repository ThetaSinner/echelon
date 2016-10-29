#include <iostream>
#include <echelon/parser/tokenizer.hpp>

#include <string>
#include <set>
#include <list>
#include <algorithm>

#include <echelon/parser/encoding-query.hpp>
#include <echelon/parser/token-expr.hpp>
#include <echelon/parser/match-data.hpp>
#include <echelon/parser/token-expr-matcher.hpp>

int main(int argc, char** args) {
  TokenExpr mlc;
  mlc.getTokenExprInfo() -> setFriendlyName("multi line comment");
  mlc.setTriggerOrdered(true);
  mlc.addTriggerChar('/');
  mlc.addTriggerChar('*');

  mlc.setContentAny(true);

  mlc.setTerminateOrdered(true);
  mlc.addTerminateChar('*');
  mlc.addTerminateChar('/');

  TokenExpr slc;
  slc.getTokenExprInfo() -> setFriendlyName("single line comment");
  slc.setTriggerOrdered(true);
  slc.addTriggerChar('/');
  slc.addTriggerChar('/');

  slc.setContentAny(true);

  slc.setTerminateOnEol(true);

  TokenExpr identifier;
  identifier.getTokenExprInfo() -> setFriendlyName("identifier");
  identifier.addTriggerChars("a-z");
  identifier.addTriggerChars("A-Z");

  identifier.addContentChars("a-z");
  identifier.addContentChars("A-Z");
  identifier.addContentChars("0-9");
  identifier.addContentChar('_');

  TokenExpr assign;
  assign.getTokenExprInfo() -> setFriendlyName("assignment");
  assign.setTriggerOrdered(true);
  assign.addTriggerChar('=');

  TokenExpr equality;
  equality.getTokenExprInfo() -> setFriendlyName("equality");
  equality.setTriggerOrdered(true);
  equality.addTriggerChar('=');
  equality.addTriggerChar('=');

  std::list<TokenExpr*> matchers2;
  matchers2.push_back(&mlc);
  matchers2.push_back(&slc);
  matchers2.push_back(&identifier);
  matchers2.push_back(&assign);
  matchers2.push_back(&equality);

  TokenExprMatcher tokenExprMatcher;
  EncodingQuery encodingQuery;

  std::string str = "helloVariable thing bit stuff = == === /* hello */ /**/ // hey\n test words";
  int linesBreaks = 0;
  int pos = 0;
  while (pos < str.size()) {
    if (str.at(pos) == ' ') {
      pos++;
      continue;
    }
    if (encodingQuery.isEol(str.at(pos))) {
      linesBreaks++;
      pos++;
      continue;
    }

    std::list<MatchData*> matches;
    for (auto& i : matchers2) {
      MatchData* matchData = tokenExprMatcher.match(i, str, pos);

      if (matchData -> isMatch()) {
        std::cout << i -> getTokenExprInfo() -> getFriendlyName() << ", ";
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
