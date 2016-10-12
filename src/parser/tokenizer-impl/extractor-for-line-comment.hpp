#ifndef LINE_COMMENT_EXTRACTOR_INCLUDED
#define LINE_COMMENT_EXTRACTOR_INCLUDED

#include "extractor.hpp"

class LineCommentExtractor : public Extractor {
  bool is_applicable(char apply_char) override;
  Optional<std::string> extract(ParseData parseData) override;
};

#endif
