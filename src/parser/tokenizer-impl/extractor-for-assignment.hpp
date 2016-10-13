#ifndef ASSIGNMENT_EXTRACTOR_INCLUDED
#define ASSIGNMENT_EXTRACTOR_INCLUDED

#include "extractor.hpp"

class AssignmentExtractor : public Extractor {
  bool is_applicable(char apply_char) override;
  Optional<Token> extract(ParseData parseData) override;
};

#endif
