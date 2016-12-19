#include <echelon/parser/stage1/character-pattern-element.hpp>

CharacterPatternElement::CharacterPatternElement(std::string data) : data(data) {
  matcher = CharacterMatcherLookup::getInstance() -> getMatcher(data);

  if (data == "any") {
    useLookahead = true;
  }
}

std::string CharacterPatternElement::getData() {
  return data;
}

CharacterMatcher CharacterPatternElement::getMatcher() {
  return matcher;
}

bool CharacterPatternElement::isUseLookahead() {
  return useLookahead;
}

