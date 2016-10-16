#ifndef echelon_TOKENIZER_INCLUDED
#define echelon_TOKENIZER_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <echelon/parser/token.hpp>

class Tokenizer
{
public:
  std::vector<Token> tokenize(std::string in);
};

#endif
