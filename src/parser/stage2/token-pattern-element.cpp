#include <echelon/parser/stage2/token-pattern-element.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#endif

#include <echelon/parser/stage2/nested-pattern-lookup.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>

TokenPatternElement::TokenPatternElement(std::string element) {
  data = element;

  if (NestedPatternLookup::getInstance() -> isNest(element)) {
    #ifdef ECHELON_DEBUG
    std::cout << "This is a nested pattern.\n";
    #endif
  }
  else {
    // Get the appropriate matcher.
    matcher = MatcherLookup::getInstance() -> getMatcher(data);
  }

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
