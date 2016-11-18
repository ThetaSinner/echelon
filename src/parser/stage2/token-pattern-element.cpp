#include <echelon/parser/stage2/token-pattern-element.hpp>

#include <echelon/parser/stage2/matcher-lookup.hpp>

TokenPatternElement::TokenPatternElement(std::string element) {
  data = element;

  // Get the appropriate matcher.
  matcher = MatcherLookup::getInstance() -> getMatcher(data);

  if (data == "block") {
    subProcess = true;
  }
}

std::string TokenPatternElement::getData() const {
  return data;
}

Matcher* TokenPatternElement::getMatcher() const {
  return matcher;
}

bool TokenPatternElement::isSubProcess() {
  return subProcess;
}
