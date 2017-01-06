#include <echelon/parser/stage1/character-pattern-group.hpp>

void CharacterPatternGroup::addElement(CharacterPatternElement *cpe) {
  elements.push_back(cpe);
}

std::list<CharacterPatternElement *> *CharacterPatternGroup::getElements() {
  return &elements;
}

void CharacterPatternGroup::setRepeatLowerBound(int repeatLowerBound) {
  this->repeatLowerBound = repeatLowerBound;
}

int CharacterPatternGroup::getRepeatLowerBound() {
  return repeatLowerBound;
}

void CharacterPatternGroup::setRepeatUpperBound(int repeatUpperBound) {
  this->repeatUpperBound = repeatUpperBound;
}

int CharacterPatternGroup::getRepeatUpperBound() {
  return repeatUpperBound;
}

void CharacterPatternGroup::setDoNotConsumeConsume(bool doNotConsumeConsume) {
  this->doNotConsumeConsume = doNotConsumeConsume;
}

bool CharacterPatternGroup::isDoNotConsumeConsume() {
  return doNotConsumeConsume;
}

void CharacterPatternGroup::setType(CharacterPatternGroupType type) {
  this->type = type;
}

CharacterPatternGroupType CharacterPatternGroup::getType() {
  return type;
}