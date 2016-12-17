#ifndef TOKENIZER_HPP_INCLUDED
#define TOKENIZER_HPP_INCLUDED

#include <list>
#include <string>

#include <echelon/parser/stage1/character-pattern-group-type-enum.hpp>
#include <echelon/parser/stage1/character-pattern-group.hpp>
#include <echelon/parser/stage1/character-pattern.hpp>
#include <echelon/parser/token.hpp>

std::list<Token*> tokenize(std::string input, std::list<CharacterPattern*> patternList);

#endif
