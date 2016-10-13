#include "extractor-for-strings.hpp"

#include "character-query.hpp"

bool StringExtractor::is_applicable(char apply_char) {
  return CharacterQuery::is_string_delim(apply_char);
};

Optional<Token> StringExtractor::extract(ParseData parseData) {
  int offset = 1;
  while (parseData.getParsePos() + offset < parseData.getParseLen()) {
  	if (CharacterQuery::is_string_delim(parseData.getParseStr().at(parseData.getParsePos() + offset))) {
  		break;
  	}
  	offset++;
  }

  return Optional<Token>::of(Token(parseData.getParseStr().substr(parseData.getParsePos(), offset + 1), TokenTypeEnum::String)); // will crash if string not closed by end of program
};
