#include "extractor-for-equality.hpp"

bool EqualityExtractor::is_applicable(char apply_char) {
  return apply_char == '=';
}

Optional<Token> EqualityExtractor::extract(ParseData parseData) {
  if (parseData.hasNext() && parseData.getNext() == '=') {
    return Optional<Token>::of(Token(parseData.getParseStr().substr(parseData.getParsePos(), 2), TokenTypeEnum::Equality));
  }

  return Optional<Token>::absent();
}
