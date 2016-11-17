#include <echelon/parser/stage2/matcher.hpp>

void Matcher::setMatcher(std::function<bool()> matcher) {
  this -> matcher = matcher;
}

bool Matcher::matches(EnhancedToken* enhancedToken) {
  this -> enhancedToken = enhancedToken;
  return matcher();
}

EnhancedToken* Matcher::getEnhancedToken() {
  return enhancedToken;
}
