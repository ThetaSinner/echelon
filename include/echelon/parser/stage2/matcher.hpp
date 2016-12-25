#ifndef MATCHER_HPP_INCLUDED
#define MATCHER_HPP_INCLUDED

#include <functional>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/parser/token-type-enum.hpp>

class Matcher {
  std::function<bool(Matcher*)> matcher;

  EnhancedToken* enhancedToken;
public:
  Matcher(std::function<bool(Matcher*)> matcher);

  static Matcher* forTokenType(TokenType tokenType);

  void setMatcher(std::function<bool(Matcher*)> matcher);

  bool matches(EnhancedToken* enhancedToken);

  EnhancedToken* getEnhancedToken();
};

#endif
