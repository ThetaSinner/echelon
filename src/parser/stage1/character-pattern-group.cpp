#include <echelon/parser/stage1/character-pattern-group.hpp>

void CharacterPatternGroup::addElement(CharacterPatternElement* cpe) {
  elements.push_back(cpe);
}

std::list<CharacterPatternElement*>* CharacterPatternGroup::getElements() {
  return &elements;
}

void CharacterPatternGroup::setRepeatable(bool repeatable) {
  this -> repeatable = repeatable;
}

bool CharacterPatternGroup::isRepeatable() {
  return repeatable;
}

void CharacterPatternGroup::setType(CharacterPatternGroupType type) {
  this -> type = type;
}

CharacterPatternGroupType CharacterPatternGroup::getType() {
  return type;
}