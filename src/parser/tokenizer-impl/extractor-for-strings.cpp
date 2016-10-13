#include "extractor-for-strings.hpp"

#include "character-query.hpp"

bool StringExtractor::is_applicable(char apply_char) {
  return CharacterQuery::is_string_delim(apply_char);
};

Optional<std::string> StringExtractor::extract(ParseData parseData) {
  int offset = 1;
  while (parseData.getParsePos() + offset < parseData.getParseLen()) {
  	if (CharacterQuery::is_string_delim(parseData.getParseStr().at(parseData.getParsePos() + offset))) {
  		break;
  	}
  	offset++;
  }

  return Optional<std::string>::of(parseData.getParseStr().substr(parseData.getParsePos(), offset + 1)); // will crash if string not closed by end of program
};
