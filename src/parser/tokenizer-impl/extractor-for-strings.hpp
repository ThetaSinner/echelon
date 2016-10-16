#ifndef STRING_EXTRACTOR_INCLUDED
#define STRING_EXTRACTOR_INCLUDED

#include "extractor.hpp"
#include <echelon/parser/token.hpp>

class StringExtractor : public Extractor {
  bool is_applicable(char apply_char) override;
  Optional<Token> extract(ParseData parseData) override;
};

#endif
