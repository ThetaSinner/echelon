#include "extractor-for-assignment.hpp"

bool AssignmentExtractor::is_applicable(char apply_char) {
  return apply_char == '=';
};

Optional<std::string> AssignmentExtractor::extract(ParseData parseData) {
  return Optional<std::string>::of(std::string(1, parseData.getCurrent()));
};
