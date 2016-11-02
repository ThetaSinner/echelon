#include <iostream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <vector>

#include <echelon/parser/tokenizer.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-type-enum.hpp>

std::string toString(TokenTypeEnum tokenType) {
  switch (tokenType) {
    case TokenTypeEnum::String: return "string";
    case TokenTypeEnum::Identifier: return "identifier";
    case TokenTypeEnum::Integer: return "integer";
    case TokenTypeEnum::BlockDelimO: return "block delim open";
    case TokenTypeEnum::BlockDelimC: return "block delim close";
    case TokenTypeEnum::Assign: return "assign";
    default: return "none";
  }
}

void stream_dump(std::ostream& s, std::vector<Token> tokens) {
  for (auto& i : tokens) {
    s << toString(i.getTokenType()) << " ["<< i.getData() << "], ";
  }
}

int main(int argc, char** args) {
  Tokenizer t;
  auto r = t.tokenize("module {\n    int t = 1\n}");

  stream_dump(std::cout, r);

  return 0;
}
