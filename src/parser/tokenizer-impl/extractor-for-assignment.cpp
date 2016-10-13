#include "extractor-for-assignment.hpp"

bool AssignmentExtractor::is_applicable(char apply_char) {
  return apply_char == '=';
};

Optional<Token> AssignmentExtractor::extract(ParseData parseData) {
  return Optional<Token>::of(Token(std::string(1, parseData.getCurrent()), TokenTypeEnum::Assign));
};
