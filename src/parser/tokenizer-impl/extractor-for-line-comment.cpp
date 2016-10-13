#include "extractor-for-line-comment.hpp"

#include "character-query.hpp"

bool LineCommentExtractor::is_applicable(char apply_char) {
  return apply_char == '/';
}

Optional<Token> LineCommentExtractor::extract(ParseData parseData) {
  if (parseData.hasNext() && parseData.getNext() == '/') {
    int offset = 2;
    while (parseData.getParsePos() + offset < parseData.getParseLen()) {
      if (CharacterQuery::is_line_break(parseData.getParseStr().at(parseData.getParsePos() + offset))) {
        break;
      }
      offset++;
    }

    return Optional<Token>::of(Token(parseData.getParseStr().substr(parseData.getParsePos(), offset), TokenTypeEnum::LineComment));
  }

  return Optional<Token>::absent();
}
