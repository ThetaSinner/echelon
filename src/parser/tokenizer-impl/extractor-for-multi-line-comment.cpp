#include "extractor-for-multi-line-comment.hpp"

#include <iostream>

bool MultiLineCommentExtractor::is_applicable(char apply_char) {
  return apply_char == '/';
}

Optional<std::string> MultiLineCommentExtractor::extract(ParseData parseData) {
  if (parseData.hasNext() && parseData.getNext() == '*') {
    int offset = 3;
    while (parseData.getParsePos() + offset < parseData.getParseLen()) {
      char prev = parseData.getParseStr().at(parseData.getParsePos() + offset - 1);
      char current = parseData.getParseStr().at(parseData.getParsePos() + offset);
      if (prev == '*' && current == '/') {
        break;
      }

      offset++;
    }

    return Optional<std::string>::of(parseData.getParseStr().substr(parseData.getParsePos(), offset + 1));
  }

  return Optional<std::string>::absent();
}
