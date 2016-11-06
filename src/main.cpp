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

class EchelonLookup {
private:
  std::set<std::string> dataTypeKeywordSet;
  std::set<std::string> keywordSet;

public:
  template<typename T>
  static std::string toString(T t);

  void addDataTypeKeyword(std::string dataTypeKeyword) {
    dataTypeKeywordSet.insert(dataTypeKeyword);

    keywordSet.insert(dataTypeKeyword);
  }

  bool isDataTypeKeyword(std::string str) {
    return dataTypeKeywordSet.find(str) != dataTypeKeywordSet.end();
  }

  void addKeyword(std::string keyword) {
    keywordSet.insert(keyword);
  }

  bool isKeyword(std::string str) {
    return keywordSet.find(str) != keywordSet.end();
  }
} echelonLookup;

class EnhancedToken {
private:
  TokenTypeEnum tokenType;
  std::string data;

  bool keyword = false;
  bool dataTypeKeyword;
public:
  EnhancedToken(Token t) {
    data = t.getData();
    tokenType = t.getTokenType();

    switch(tokenType) {
      case TokenTypeEnum::Identifier:
        keyword = echelonLookup.isKeyword(data);
        dataTypeKeyword = echelonLookup.isDataTypeKeyword(data);
        break;
    }
  }

  TokenTypeEnum getTokenType() {
    return tokenType;
  }

  std::string getData() {
    return data;
  }

  bool isKeyword() {
    return keyword;
  }

  bool isDataTypeKeyword() {
    return dataTypeKeyword;
  }
};

enum class Keyword {
  Package,
  Module,

  Integer
};

template<> std::string EchelonLookup::toString(Keyword t) {
  switch(t) {
    case Keyword::Package:
      return "package";
    case Keyword::Module:
      return "module";
    default:
      return "none";
  }
}

template<> std::string EchelonLookup::toString(TokenTypeEnum t) {
  switch(t) {
    case TokenTypeEnum::Identifier:
      return "identifier";
    default:
      return "none";
  }
}

template<typename T>
bool eq(T e, std::string s) {
  return EchelonLookup::toString(e) == s;
}

bool operator==(const Keyword& l, const std::string& r) {
  return eq(l, r);
}

bool operator==(const TokenTypeEnum& l, const std::string& r) {
  return eq(l, r);
}

std::string toString(bool b) {
  return b ? "true" : "false";
}

void stream_dump(std::ostream& s, std::vector<Token> tokens) {
  for (auto& i : tokens) {
    s << toString(i.getTokenType()) << " ["<< i.getData() << "], ";
  }
}

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

// scrap.
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

class TokenPatternElement {
private:
  std::string data;

  bool prefix = false;

  bool optional = false;
  int repeatLowerBound = 1;
  int repeatUpperBound = 1;
public:
  TokenPatternElement(std::string element);

  std::string getData() {
    return data;
  }

  void setPrefix(bool prefix) {
    this -> prefix = prefix;
  }
  bool isPrefix() {
    return prefix;
  }

  void setOptional(bool optional);
  bool isOptional();

  void setRepeatLowerBound(int repeatLowerBound);
  int getRepeatLowerBound();

  void setRepeatUpperBound(int repeatUpperBound);
  int getRepeatUpperBound();
};

TokenPatternElement::TokenPatternElement(std::string element) {
  data = element;

  if (element.substr(1, 3) == "kwd") {
    std::cout << element.substr(4);
  }

  if (element == "type") {
    // we are a datatype. must have token type identifier and be a valid type identifier.
  }

  if (element == "identifier") {
    // we are a non-keyword identifier. (this should be checked on input)
  }

  if (element == "assign") {
    // we are an assignment operator. must have token type assign.
  }
}

void TokenPatternElement::setOptional(bool optional) {
  setRepeatLowerBound(0);
  this -> optional = optional;
}

bool TokenPatternElement::isOptional() {
  return optional;
}

void TokenPatternElement::setRepeatLowerBound(int repeatLowerBound) {
  this -> repeatLowerBound = repeatLowerBound;
}

int TokenPatternElement::getRepeatLowerBound() {
  return repeatLowerBound;
}

void TokenPatternElement::setRepeatUpperBound(int repeatUpperBound) {
  this -> repeatUpperBound = repeatUpperBound;
}

int TokenPatternElement::getRepeatUpperBound() {
  return repeatUpperBound;
}

class TokenPattern {
private:
  std::vector<TokenPatternElement*> tokenPatternElements;

public:
  void addElement(TokenPatternElement* tokenPatternElement) {
    tokenPatternElements.push_back(tokenPatternElement);
  }

  std::vector<TokenPatternElement*>* getElements() {
    return &tokenPatternElements;
  }
};

void stream_dump(std::ostream& s, TokenPatternElement* tokenPatternElement) {
  if (tokenPatternElement -> isPrefix()) {
    s << "prefix: ";
  }
  if (tokenPatternElement -> isOptional()) {
    s << "optional: ";
  }

  s << tokenPatternElement -> getData();
  s << "{" << tokenPatternElement -> getRepeatLowerBound() << "," << tokenPatternElement -> getRepeatUpperBound() << "}";
}


void stream_dump(std::ostream& s, TokenPattern* tokenPattern) {
  for (auto& i : *tokenPattern -> getElements()) {
    stream_dump(s, i);
    s << " ";
  }
}

TokenPattern* translate_pattern(std::string pattern) {
  std::cout << "Pattern length: " << pattern.size() << "\n";

  TokenPattern *tokenPattern = new TokenPattern();

  for (auto i = pattern.begin(); i != pattern.end(); i++) {
    if (*i == ' ') {
      continue;
    }

    bool matched_optional = false;
    if (*i == '[') {
      auto it = i;
      while (*(++it) != ']') {}

      TokenPatternElement *tokenPatternElement = new TokenPatternElement(std::string(i + 1, it));
      tokenPatternElement -> setOptional(true);

      // We want to consume the ] too.
      safe_advance(i, it - i + 1, pattern);

      if (*(++it) != ' ') {
        if (*it == '*') {
          tokenPatternElement -> setRepeatUpperBound(-1);
          safe_advance(i, 1, pattern);
        }
      }

      tokenPattern -> addElement(tokenPatternElement);
      matched_optional = true;
    }

    bool matched_identifier = false;
    if (is_letter(*i)) {
      auto it = i;
      while (is_identifier_char(*(++it))) {}

      TokenPatternElement *tokenPatternElement = new TokenPatternElement(std::string(i, it));

      safe_advance(i, it - i, pattern);

      tokenPattern -> addElement(tokenPatternElement);
      matched_identifier = true;
    }

    if (matched_optional && matched_identifier) {
      tokenPattern -> getElements() -> at(tokenPattern -> getElements() -> size() - 2) -> setPrefix(true);
    }

    if (i == pattern.end()) {
      i--;
    }
  }

  return tokenPattern;
}

AstNode* parse2(std::list<Token> tokens) {
  std::stack<EnhancedToken> enchancedTokens;

  for (auto& t : tokens) {

  }
}

int main(int argc, char** args) {
  Tokenizer t;
  auto r = t.tokenize("module {\n    integer t = 1\n}");

  stream_dump(std::cout, r);

  //auto x = parse(r);

  //

  if (Keyword::Integer == "integer") {
    std::cout << "wow.\n";
  }

  echelonLookup.addDataTypeKeyword("integer");
  echelonLookup.addDataTypeKeyword("string");

  echelonLookup.addKeyword("package");

  std::cout << EchelonLookup::toString(Keyword::Module) << std::endl;
  std::cout << EchelonLookup::toString(TokenTypeEnum::Identifier) << std::endl;

  std::list<Token> program;
  Token packageKwd("package", TokenTypeEnum::Identifier);
  program.push_back(packageKwd);
  Token projectName("echelon", TokenTypeEnum::Identifier);
  program.push_back(projectName);
  Token structureOperator("::", TokenTypeEnum::StructureOperator);
  program.push_back(structureOperator);
  Token packageName("test_package", TokenTypeEnum::Identifier);
  program.push_back(packageName);


  // may contain the toString of any token type.
  std::string assignment_expr = "[type] identifier assign [type_]expr";
  std::string for_loop = "kwd_for [type] identifier assign expr; bool_expr; expr block_delim_o [block] block_delim_c";
  std::string package = "kwd_package [identifier_structure]*identifier";

  std::cout << "\n";
  auto assignment_pattern = translate_pattern(assignment_expr);
  stream_dump(std::cout, assignment_pattern);
  std::cout << "\n";
  auto for_loop_pattern = translate_pattern(for_loop);
  stream_dump(std::cout, for_loop_pattern);
  std::cout << "\n";
  auto package_pattern = translate_pattern(package);
  stream_dump(std::cout, package_pattern);
  std::cout << "\n";

  return 0;
}
