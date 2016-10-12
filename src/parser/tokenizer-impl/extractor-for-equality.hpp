#ifndef EQUALTIY_EXTRACTOR_INCLUDED
#define EQUALTIY_EXTRACTOR_INCLUDED

#include "extractor.hpp"

class EqualityExtractor : public Extractor {
  bool is_applicable(char apply_char) override;
  Optional<std::string> extract(ParseData parseData) override;
};

#endif
