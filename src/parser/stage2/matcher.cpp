#include <echelon/parser/stage2/matcher.hpp>

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
