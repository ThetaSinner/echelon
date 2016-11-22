#include <echelon/parser/stage2/token-pattern-element.hpp>

#include <echelon/parser/stage2/nested-pattern-lookup.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>

TokenPatternElement::TokenPatternElement(std::string element) {
  data = element;

  if (NestedPatternLookup::getInstance() -> isNest(element)) {
    nestedPatterns = NestedPatternLookup::getInstance() -> getNested(element);
    useNestedPatterns = true;
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

std::list<TokenPattern*>* TokenPatternElement::getNestedPatterns() const {
  return nestedPatterns;
}

bool TokenPatternElement::isSubProcess() {
  return subProcess;
}

bool TokenPatternElement::isUseNestedPatterns() {
  return useNestedPatterns;
}
