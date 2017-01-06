#include <echelon/parser/stage1/character-pattern-parser.hpp>

bool isIdent(char c) {
  return (97 <= c && c <= 122) || (48 <= c && c <= 57) || c == '_';
}

CharacterPattern *parseCharacterPattern(std::string pattern) {
  CharacterPattern *characterPattern = new CharacterPattern();

  int strPos = 0;
  int strLen = pattern.size();
  while (strPos < strLen) {
    auto currentChar = pattern[strPos];

    if (currentChar == '(') {
      int subStrPos = strPos + 1;

      CharacterPatternGroup *group = new CharacterPatternGroup();
      group->setType(CharacterPatternGroupType::Union);
      while (subStrPos < strLen && pattern[subStrPos] != ')') {
        int start = subStrPos;
        while (isIdent(pattern[subStrPos])) {
          subStrPos++;
        }

        group->addElement(new CharacterPatternElement(pattern.substr(start, subStrPos - start)));
        if (pattern[subStrPos] == ' ') {
          subStrPos++;
        }
      }

      if (pattern[subStrPos] == ')') {
        subStrPos++;
      }

      if (pattern[subStrPos] == '*') {
        group->setRepeatLowerBound(0);
        group->setRepeatUpperBound(-1);
        subStrPos++;
      }

      if (pattern[subStrPos] == '+') {
        group->setRepeatLowerBound(1);
        group->setRepeatUpperBound(-1);
        subStrPos++;
      }

      characterPattern->addGroup(group);
      strPos = subStrPos;
      continue;
    }

    if (currentChar == '[') {
      int subStrPos = strPos + 1;

      CharacterPatternGroup *group = new CharacterPatternGroup();
      group->setType(CharacterPatternGroupType::Sequence);
      while (subStrPos < strLen && pattern[subStrPos] != ']') {
        int start = subStrPos;
        while (isIdent(pattern[subStrPos])) {
          subStrPos++;
        }

        group->addElement(new CharacterPatternElement(pattern.substr(start, subStrPos - start)));
        if (pattern[subStrPos] == ' ') {
          subStrPos++;
        }
      }

      if (pattern[subStrPos] == ']') {
        subStrPos++;
      }

      characterPattern->addGroup(group);
      strPos = subStrPos;
      continue;
    }

    if (isIdent(pattern[strPos])) {
      int start = strPos;
      int subStrPos = strPos;
      while (isIdent(pattern[subStrPos])) {
        subStrPos++;
      }

      CharacterPatternElement *cpe = new CharacterPatternElement(pattern.substr(start, subStrPos - start));

      CharacterPatternGroup *cpg = new CharacterPatternGroup();
      cpg->addElement(cpe);

      characterPattern->addGroup(cpg);

      if (pattern[subStrPos] == '*') {
        cpg->setRepeatLowerBound(0);
        cpg->setRepeatUpperBound(-1);
        subStrPos++;
      }

      if (pattern[subStrPos] == '+') {
        cpg->setRepeatLowerBound(1);
        cpg->setRepeatUpperBound(-1);
        subStrPos++;
      }

      if (pattern[subStrPos] == '`') {
        cpg->setDoNotConsumeConsume(true);
        subStrPos++;
      }

      strPos = subStrPos;
      continue;
    }

    strPos++;
  }

  return characterPattern;
}