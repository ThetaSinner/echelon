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
  if (it -> getTokenType() == TokenTypeEnum::Integer) {
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

bool is_letter(int c) {
  return (c >= 97) && (c <= 122);
}

bool is_identifier_char(int c) {
  return is_letter(c) || c == '_';
}

bool safe_advance(std::string::iterator& it, int n, std::string& s) {
  while (n) {
    if (it != s.end()) {
      it++;
    }
    else {
      return false;
    }
    n--;
  }

  return true;
}

void translate_pattern(std::string pattern) {
  std::cout << "Pattern length: " << pattern.size() << "\n";

  std::cout << "[";
  for (auto i = pattern.begin(); i != pattern.end(); i++) {
    if (*i == ' ') {
      // new expression.
      std::cout << "][";
      continue;
    }

    bool iterator_moved = false;

    if (*i == '[') {
      iterator_moved = true;

      auto it = i;
      while (*(++it) != ']') {}

      std::cout << "'optional: " << std::string(i + 1, it) << " ";

      if (!safe_advance(i, it - i, pattern)) {
        break;
      }

      if (*(++it) != ' ') {
        if (*it == '*') {
          std::cout << "may repeat ";
          if (!safe_advance(i, it - i, pattern)) {
            break;
          }
        }
      }

      std::cout << "' ";
    }

    if (is_letter(*i)) {
      iterator_moved = true;

      auto it = i;
      while (is_identifier_char(*(++it))) {}

      std::cout << "'" << std::string(i, it) << "' ";

      if (!safe_advance(i, it - i, pattern)) {
        //
        break;
      }
    }

    if (iterator_moved) {
      i--;
    }
  }
  std::cout << "]";
}

int main(int argc, char** args) {
  Tokenizer t;
  auto r = t.tokenize("module {\n    integer t = 1\n}");

  stream_dump(std::cout, r);

  //auto x = parse(r);

  //

  std::string assignment_expr = "[type] identifier assign [type_]expr";
  std::string for_loop = "kwd_for [type] identifier assign expr; bool_expr; expr block_delim_o block block_delim_c";
  std::string package = "kwd_package [identifier_scope]*identifier";

  std::cout << "\n";
  translate_pattern(assignment_expr);
  std::cout << "\n";
  translate_pattern(for_loop);
  std::cout << "\n";
  translate_pattern(package);

  return 0;
}
