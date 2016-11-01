#include <echelon/parser/tokenizer.hpp>

#include <echelon/optional.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-expr-matcher.hpp>

#include <list>

#include "tokenizer-impl/character-query.hpp"
#include "tokenizer-impl/extractor-for-equality.hpp"
#include "tokenizer-impl/extractor-for-assignment.hpp"
#include "tokenizer-impl/extractor-for-line-comment.hpp"
#include "tokenizer-impl/extractor-for-multi-line-comment.hpp"
#include "tokenizer-impl/extractor-for-strings.hpp"

Tokenizer::Tokenizer() {
  TokenExpr mlc;
  mlc.getTokenExprInfo() -> setFriendlyName("multi line comment");
  mlc.setTriggerOrdered(true);
  mlc.addTriggerChar('/');
  mlc.addTriggerChar('*');

  mlc.setContentAny(true);

  mlc.setTerminateOrdered(true);
  mlc.addTerminateChar('*');
  mlc.addTerminateChar('/');

  tokenExprMap.insert({TokenTypeEnum::MultiLineComment, mlc});

  TokenExpr slc;
  slc.getTokenExprInfo() -> setFriendlyName("single line comment");
  slc.setTriggerOrdered(true);
  slc.addTriggerChar('/');
  slc.addTriggerChar('/');

  slc.setContentAny(true);

  slc.setTerminateOnEol(true);

  tokenExprMap.insert({TokenTypeEnum::SingleLineComment, slc});

  TokenExpr identifier;
  identifier.getTokenExprInfo() -> setFriendlyName("identifier");
  identifier.addTriggerChars("a-z");
  identifier.addTriggerChars("A-Z");

  identifier.addContentChars("a-z");
  identifier.addContentChars("A-Z");
  identifier.addContentChars("0-9");
  identifier.addContentChar('_');

  tokenExprMap.insert({TokenTypeEnum::Identifier, identifier});

  TokenExpr assign;
  assign.getTokenExprInfo() -> setFriendlyName("assignment");
  assign.setTriggerOrdered(true);
  assign.addTriggerChar('=');

  tokenExprMap.insert({TokenTypeEnum::Assign, assign});

  TokenExpr equality;
  equality.getTokenExprInfo() -> setFriendlyName("equality");
  equality.setTriggerOrdered(true);
  equality.addTriggerChar('=');
  equality.addTriggerChar('=');

  tokenExprMap.insert({TokenTypeEnum::Equality, equality});
}

std::vector<Token> Tokenizer::tokenize(std::string in) {
  //std::cout << "Tokenizing string: " << in << std::endl;

  std::vector<Token> tokens;

  TokenExprMatcher tokenExprMatcher;

  int linesBreaks = 0;

  long str_pos = 0;
  long in_len = in.size();
  while (str_pos < in_len) {
    char current_char = in.at(str_pos);

    if (CharacterQuery::is_space(current_char)) {
      str_pos++;
      continue;
    }

    if (CharacterQuery::is_end_of_line(current_char)) {
      linesBreaks++;
      str_pos++;
      continue;
    }

    std::list<MatchData*> matches;
    for (auto& i : tokenExprMap) {
      MatchData* matchData = tokenExprMatcher.match(&i.second, in, str_pos);
      matchData -> setTokenTypeEnum(i.first);

      if (matchData -> isMatch()) {
        std::cout << (i.second).getTokenExprInfo() -> getFriendlyName() << ", ";
        matches.push_back(matchData);
      }
    }

    if (matches.size()) {
      matches.sort([](MatchData* a, MatchData* b) {
        return a -> getLen() < b -> getLen();
      });

      int extract = matches.back() -> getLen();
      Token token(in.substr(str_pos, extract), matches.back() -> getTokenTypeEnum());
      tokens.push_back(token);
      str_pos += extract;
    }
    else {
      std::string err = "Unhandled character + [" + std::string(1, current_char) + "] rest is [" + in.substr(str_pos);
      throw std::domain_error(err);
    }
  }

  return tokens;
}

std::map<TokenTypeEnum, TokenExpr> Tokenizer::getTokenExprMap() {
  return tokenExprMap;
}
