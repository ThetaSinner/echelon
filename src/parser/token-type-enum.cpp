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
