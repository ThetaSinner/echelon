#ifndef ELM_TOKENIZER_INCLUDED
#define ELM_TOKENIZER_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>

#include <elm/parser/character-query.hpp>

class Tokenizer
{
public:
  std::vector<std::string> tokenize(std::string in);
};

#endif
