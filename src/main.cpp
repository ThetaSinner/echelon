#include <iostream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <vector>
#include <stack>
#include <functional>

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

class Matcher {
  std::function<bool()> matcher;

  EchelonLookup* echelonLookup;

  EnhancedToken* enhancedToken;
public:
  void setMatcher(std::function<bool()> matcher) {
    this -> matcher = matcher;
  }

  void setLookup(EchelonLookup* echelonLookup) {
    this -> echelonLookup = echelonLookup;
  }
  EchelonLookup* getLookup() {
    return echelonLookup;
  }

  bool matches(EnhancedToken* enhancedToken) {
    this -> enhancedToken = enhancedToken;
    return matcher();
  }

  EnhancedToken* getEnhancedToken() {
    return enhancedToken;
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
public:
  TokenPatternElement(std::string element);

  std::string getData() {
    return data;
  }
};

TokenPatternElement::TokenPatternElement(std::string element) {
  data = element;

  // Get the appropriate matcher and expose it.

  if (element.substr(1, 3) == "kwd") {
    // element.substr(4);
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

class TokenPatternGroup {
  std::vector<TokenPatternElement*> tokenPatternElements;

  int repeatLowerBound = 1;
  int repeatUpperBound = 1;
public:
  void addElement(TokenPatternElement* tokenPatternElement) {
    tokenPatternElements.push_back(tokenPatternElement);
  }

  std::vector<TokenPatternElement*>* getElements() {
    return &tokenPatternElements;
  }

  void setRepeatLowerBound(int repeatLowerBound) {
    this -> repeatLowerBound = repeatLowerBound;
  }
  int getRepeatLowerBound() {
    return repeatLowerBound;
  }

  void setRepeatUpperBound(int repeatUpperBound) {
    this -> repeatUpperBound = repeatUpperBound;
  }
  int getRepeatUpperBound() {
    return repeatUpperBound;
  }
};

class TokenPattern {
  std::vector<TokenPatternGroup*> tokenPatternGroups;
public:
  void addGroup(TokenPatternGroup* tokenPatternGroup) {
    tokenPatternGroups.push_back(tokenPatternGroup);
  }

  std::vector<TokenPatternGroup*>* getGroups() {
    return &tokenPatternGroups;
  }
};

void stream_dump(std::ostream& s, TokenPatternElement* tokenPatternElement) {
  s << tokenPatternElement -> getData();
  //s << "{" << tokenPatternElement -> getRepeatLowerBound() << "," << tokenPatternElement -> getRepeatUpperBound() << "}";
}

void stream_dump(std::ostream& s, TokenPatternGroup* tokenPatternGroup) {
  s << "'";
  int sp = false;
  for (auto& i : *tokenPatternGroup -> getElements()) {
    if (sp) {
      s << " ";
    }
    stream_dump(s, i);
    sp = true;
  }
  s << "'{" << tokenPatternGroup -> getRepeatLowerBound() << "," << tokenPatternGroup -> getRepeatUpperBound() << "}";
}

void stream_dump(std::ostream& s, TokenPattern* tokenPattern) {
  for (auto& i : *tokenPattern -> getGroups()) {
    stream_dump(s, i);
    s << " ";
  }
}

/* initial version, has been replaced.
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
*/

class Parser2 {
private:
  std::vector<TokenPattern*> tokenPatterns;

  AstNode* _parse(const std::list<Token>& tokens) {
    std::stack<EnhancedToken*> enchancedTokens;


    for (auto& t : tokens) {
      EnhancedToken *enhancedToken = new EnhancedToken(t);
      enchancedTokens.push(enhancedToken);


    }
  }
public:
  AstNode* parse(std::list<Token> tokens) {
    return _parse(tokens);
  }

  void addTokenPattern(TokenPattern* tokenPattern) {
    tokenPatterns.push_back(tokenPattern);
  }
};

/////
class PatternTranslator {
private:
  TokenPatternElement* readIdentifier(std::string::iterator& i, std::string& pattern) {
    if (is_letter(*i)) {
      auto it = i;
      while (is_identifier_char(*(++it))) {}

      TokenPatternElement *tokenPatternElement = new TokenPatternElement(std::string(i, it));

      safe_advance(i, it - i, pattern);

      return tokenPatternElement;
    }

    return nullptr;
  }
public:
  TokenPattern* translate(std::string pattern);
};

TokenPattern* PatternTranslator::translate(std::string pattern) {
  std::cout << "Pattern length: " << pattern.size() << "\n";

  TokenPattern *tokenPattern = new TokenPattern();

  for (auto i = pattern.begin(); i != pattern.end(); i++) {
    TokenPatternGroup *tokenPatternGroup = new TokenPatternGroup();

    if (*i == '[') {
      // is optional.
      tokenPatternGroup -> setRepeatLowerBound(0);

      i++;
      while (true) {
        if (*i == ' ') {
          i++;
          continue;
        }

        auto ident = readIdentifier(i, pattern);

        if (ident == nullptr) {
          break;
        }

        tokenPatternGroup -> addElement(ident);
      }

      while (*i == ' ') {
        i++;
      }
      if (*i != ']') {
        std::cout << "(0042) syntax error.\n";
      }
      // consume the ']'
      safe_advance(i, 1, pattern);

      if (*i == '*') {
        tokenPatternGroup -> setRepeatUpperBound(-1);
        safe_advance(i, 1, pattern);
      }
    }
    else if (is_letter(*i)) {
      auto ident = readIdentifier(i, pattern);
      tokenPatternGroup -> addElement(ident);
    }
    else {
      std::cout << "(0041) syntax error." << *i << "\n";
      break;
    }

    tokenPattern -> addGroup(tokenPatternGroup);
    if (i == pattern.end()) {
      i--;
    }
  }

  return tokenPattern;
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
  std::string var_decl = "[type] identifier assign";
  std::string assignment_expr = "[type] identifier assign expr";
  std::string for_loop = "kwd_for [type] identifier assign expr; bool_expr; expr block_delim_o [block] block_delim_c";
  std::string package = "kwd_package [identifier op_structure]* identifier";

  std::cout << "\n";
  stream_dump(std::cout, (new PatternTranslator()) -> translate(var_decl));
  std::cout << "\n";
  stream_dump(std::cout, (new PatternTranslator()) -> translate(assignment_expr));
  std::cout << "\n";
  stream_dump(std::cout, (new PatternTranslator()) -> translate(for_loop));
  std::cout << "\n";
  stream_dump(std::cout, (new PatternTranslator()) -> translate(package));
  std::cout << "\n";

  Matcher* type = new Matcher();
  type -> setLookup(&echelonLookup);
  type -> setMatcher([&type] () -> bool {
    if (type -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return type -> getLookup() -> isDataTypeKeyword(type -> getEnhancedToken() -> getData());
  });

  Matcher* keyword = new Matcher();
  keyword -> setLookup(&echelonLookup);
  keyword -> setMatcher([&keyword] () -> bool {
    if (keyword -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return keyword -> getLookup() -> isKeyword(keyword -> getEnhancedToken() -> getData());
  });

  EnhancedToken *enhancedPackageKwd = new EnhancedToken(packageKwd);

  std::cout << toString(keyword -> matches(enhancedPackageKwd)) << "\n";
  std::cout << toString(type -> matches(enhancedPackageKwd)) << "\n";

  return 0;
}
