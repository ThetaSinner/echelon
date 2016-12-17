#include <echelon/parser/stage1/character-pattern-lookup.hpp>

CharacterPatternLookup* CharacterPatternLookup::instance = nullptr;

CharacterPatternLookup* CharacterPatternLookup::getInstance() {
    if (instance == nullptr) {
        instance = new CharacterPatternLookup();
    }

    return instance;
}

void CharacterPatternLookup::addCharacterPattern(CharacterPattern* characterPattern) {
    characterPatternList.push_back(characterPattern);
}

std::list<CharacterPattern*>* CharacterPatternLookup::getCharacterPatternList() {
    return &characterPatternList;
}