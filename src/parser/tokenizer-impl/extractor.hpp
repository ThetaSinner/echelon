#ifndef EXTRACTOR_INCLUDED
#define EXTRACTOR_INCLUDED

#include <string>
#include <elm/optional.hpp>
#include <elm/parser/token.hpp>
#include "parse-data.hpp"

class Extractor {
public:
  virtual bool is_applicable(char apply_char) = 0;
  virtual Optional<Token> extract(ParseData parseData) = 0;
};

#endif
