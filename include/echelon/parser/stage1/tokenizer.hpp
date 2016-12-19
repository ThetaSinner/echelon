#ifndef TOKENIZER_HPP_INCLUDED
#define TOKENIZER_HPP_INCLUDED

#include <vector>
#include <string>

#include <echelon/parser/stage1/character-pattern-group-type-enum.hpp>
#include <echelon/parser/stage1/character-pattern-group.hpp>
#include <echelon/parser/stage1/character-pattern.hpp>
#include <echelon/parser/stage1/token.hpp>

class Tokenizer {
public:
  std::list<Token*> tokenize(std::string input);
};

#endif
