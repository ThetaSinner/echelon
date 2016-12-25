#include <echelon/parser/stage2/matcher.hpp>

Matcher::Matcher(std::function<bool(Matcher*)> matcher) {
  setMatcher(matcher);
}

Matcher* Matcher::forTokenType(TokenType tokenType) {
  return new Matcher([tokenType] (Matcher* matcher) -> bool {
    return matcher -> getEnhancedToken() -> getTokenType() == tokenType;
  });
}

void Matcher::setMatcher(std::function<bool(Matcher*)> matcher) {
  this -> matcher = matcher;
}

bool Matcher::matches(EnhancedToken* enhancedToken) {
  this -> enhancedToken = enhancedToken;
  return matcher(this);
}

EnhancedToken* Matcher::getEnhancedToken() {
  return enhancedToken;
}
