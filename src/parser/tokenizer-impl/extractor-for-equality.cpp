#include "extractor-for-equality.hpp"

bool EqualityExtractor::is_applicable(char apply_char) {
  return apply_char == '=';
}

Optional<std::string> EqualityExtractor::extract(ParseData parseData) {
  if (parseData.hasNext() && parseData.getNext() == '=') {
    return Optional<std::string>::of(parseData.getParseStr().substr(parseData.getParsePos(), 2));
  }

  return Optional<std::string>::absent();
}
