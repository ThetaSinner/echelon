#ifndef ELM_TOKENIZER_INCLUDED
#define ELM_TOKENIZER_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <elm/parser/token.hpp>

class Tokenizer
{
public:
  std::vector<Token> tokenize(std::string in);
};

#endif
