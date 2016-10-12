#ifndef MULTI_LINE_COMMENT_EXTRACTOR_INCLUDED
#define MULTI_LINE_COMMENT_EXTRACTOR_INCLUDED

#include "extractor.hpp"

class MultiLineCommentExtractor : public Extractor {
  bool is_applicable(char apply_char) override;
  Optional<std::string> extract(ParseData parseData) override;
};

#endif
