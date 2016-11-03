#include <iostream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <vector>
#include <stack>

#include <echelon/parser/tokenizer.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-type-enum.hpp>

#include <echelon/ast/AstNode.hpp>
#include <echelon/ast/AstNodeType.hpp>

std::string toString(bool b) {
  return b ? "true" : "false";
}

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

enum class Keyword {
  Module,

  Integer
};

class ParserData {
private:
  std::map<Keyword, std::string> keywordStringMap;
  std::set<std::string> keywordStringSet;
  std::set<std::string> dataTypeIdentifierKeywordStringSet;
public:
  ParserData() {
    keywordStringMap.insert({Keyword::Integer, "integer"});

    for (auto& i : keywordStringMap) {
      keywordStringSet.insert(i.second);
    }

    dataTypeIdentifierKeywordStringSet.insert(keywordStringMap.at(Keyword::Integer));
  }

  std::set<std::string>* getKeywordStringSet() {
    return &keywordStringSet;
  }

  std::set<std::string>* getDataTypeIdentifierKeywordStringSet() {
    return &dataTypeIdentifierKeywordStringSet;
  }

} parserData;

std::string toString(Keyword keyword) {
  switch (keyword) {
    case Keyword::Module: return "module";
    case Keyword::Integer: return "integer";
    default: return "none";
  }
}

bool isDataTypeIdentifier(Token t) {
  if (t.getTokenType() == TokenTypeEnum::Identifier) {
    return parserData.getDataTypeIdentifierKeywordStringSet() -> find(t.getData())
      != parserData.getDataTypeIdentifierKeywordStringSet() -> end();
  }

  return false;
}

bool isKeyword(Token t) {
  if (t.getTokenType() == TokenTypeEnum::Identifier) {
    return parserData.getKeywordStringSet() -> find(t.getData())
      != parserData.getKeywordStringSet() -> end();
  }

  return false;
}

void process_dataTypeIdentifier(AstNode *node, std::vector<Token>::iterator it) {
  AstNode* process_node = new AstNode();
  process_node -> setType(AstNodeType::AssignmentExpr);

  auto dataType = *it;
  AstNode* type_node = new AstNode();
  type_node -> setType(AstNodeType::ExplicitType);

  it++;
  auto identifier = *it;
  std::cout << "identifier " << identifier.getData() << "\n";
  if (identifier.getTokenType() != TokenTypeEnum::Identifier) {
    std::cout << "expected identifier\n";
  }
  if (isKeyword(identifier)) {
    std::cout << "cannot use keyword as identifier\n";
  }
  // add identifier to tree.

  it++;
  auto assign = *it;
  std::cout << "assign " << assign.getData() << "\n";
  if (assign.getTokenType() != TokenTypeEnum::Assign) {
    std::cout << "expected equals sign\n";
  }
  // modify tree.

  it++;
  if (it.getTokenType() == TokenTypeEnum::Integer) {
    // add value to tree.
  }
  else {
    std::cout << "expected value";
  }
}

AstNode* parse(std::vector<Token> tokens) {
  AstNode *node = new AstNode();

  for (auto i = tokens.begin(); i != tokens.end(); i++) {
    if (isDataTypeIdentifier(*i)) {
      std::cout << i -> getData() << " is data type identifier\n";
      process_dataTypeIdentifier(node, i);
    }
  }
}

int main(int argc, char** args) {
  Tokenizer t;
  auto r = t.tokenize("module {\n    integer t = 1\n}");

  stream_dump(std::cout, r);

  auto x = parse(r);

  return 0;
}
