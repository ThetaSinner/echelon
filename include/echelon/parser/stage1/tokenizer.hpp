#ifndef TOKENIZER_HPP_INCLUDED
#define TOKENIZER_HPP_INCLUDED

#include <list>
#include <string>

#include <echelon/parser/stage1/character-pattern-group-type-enum.hpp>
#include <echelon/parser/stage1/character-pattern-group.hpp>
#include <echelon/parser/stage1/character-pattern.hpp>

void tokenize(std::string input, std::list<CharacterPattern*> patternList);

#endif
