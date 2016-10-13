#ifndef STRING_EXTRACTOR_INCLUDED
#define STRING_EXTRACTOR_INCLUDED

#include "extractor.hpp"

class StringExtractor : public Extractor {
  bool is_applicable(char apply_char) override;
  Optional<std::string> extract(ParseData parseData) override;
};

#endif
