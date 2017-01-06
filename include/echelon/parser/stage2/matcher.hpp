#ifndef MATCHER_HPP_INCLUDED
#define MATCHER_HPP_INCLUDED

#include <functional>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>

class Matcher {
  std::function<bool(Matcher *)> matcher;

  EnhancedToken *enhancedToken;
public:
  Matcher(std::function<bool(Matcher *)> matcher);

  static Matcher *forTokenType(TokenType tokenType);

  static Matcher *forKeyword(Keyword keyword);

  void setMatcher(std::function<bool(Matcher *)> matcher);

  bool matches(EnhancedToken *enhancedToken);

  EnhancedToken *getEnhancedToken();
};

#endif
