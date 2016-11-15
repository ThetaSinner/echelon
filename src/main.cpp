#include <iostream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <vector>
#include <stack>
#include <functional>
#include <iterator>

#include <echelon/parser/tokenizer.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-type-enum.hpp>

#include <echelon/ast/AstNode.hpp>
#include <echelon/ast/AstNodeType.hpp>

#define ECHELON_DEBUG

class EchelonLookup {
private:
  static EchelonLookup *self;

  std::set<std::string> dataTypeKeywordSet;
  std::set<std::string> keywordSet;

  EchelonLookup() {}
  EchelonLookup(const EchelonLookup& _) {}
  void operator=(const EchelonLookup& _) {}
public:
  static EchelonLookup* getInstance() {
    if (self == nullptr) {
      self = new EchelonLookup();
    }

    return self;
  }

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
};

EchelonLookup* EchelonLookup::self = nullptr;

template<> std::string EchelonLookup::toString(TokenTypeEnum t) {
  switch(t) {
    case TokenTypeEnum::Identifier:
      return "identifier";
    default:
      return "none";
  }
}

template<> std::string EchelonLookup::toString(AstNodeType t) {
  switch(t) {
    case AstNodeType::Package:
      return "package";
    default:
      return "none";
  }
}

template<typename T>
bool eq(T e, std::string s) {
  return EchelonLookup::toString(e) == s;
}

void stream_dump(std::ostream& s, const Token* t) {
  s << "{" << t -> getData() << ", " << EchelonLookup::getInstance() -> toString(t -> getTokenType()) << "}";
}

class EnhancedToken {
private:
  TokenTypeEnum tokenType;
  std::string data;

  bool keyword = false;
  bool dataTypeKeyword;
public:
  EnhancedToken(Token* t) {
    data = t -> getData();
    tokenType = t -> getTokenType();

    switch(tokenType) {
      case TokenTypeEnum::Identifier:
        keyword = EchelonLookup::getInstance() -> isKeyword(data);
        dataTypeKeyword = EchelonLookup::getInstance() -> isDataTypeKeyword(data);
        break;
      default:
        std::cout << "Unhandled case in EnhancedToken constructor.\n";
        // do nothing.
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

  EnhancedToken* enhancedToken;
public:
  void setMatcher(std::function<bool()> matcher) {
    this -> matcher = matcher;
  }

  bool matches(EnhancedToken* enhancedToken) {
    this -> enhancedToken = enhancedToken;
    return matcher();
  }

  EnhancedToken* getEnhancedToken() {
    return enhancedToken;
  }
};

class MatcherLookup {
  static MatcherLookup *self;

  std::map<std::string, Matcher*> matcherHash;

  MatcherLookup() {};
  MatcherLookup(const MatcherLookup& _) {}
  void operator=(const MatcherLookup& _) {}
public:
  static MatcherLookup* getInstance() {
    if (self == nullptr) {
      self = new MatcherLookup();
    }

    return self;
  }

  void addMatcher(std::string key, Matcher* matcher) {
    matcherHash.insert({key, matcher});
  }

  Matcher* getMatcher(std::string key) {
    #ifdef ECHELON_DEBUG
    if (matcherHash.find(key) == matcherHash.end()) {
      std::cout << "Missing matcher for " << key << std::endl;
      throw std::runtime_error("Missing matcher");
    }
    #endif

    return matcherHash.at(key);
  }
};

MatcherLookup *MatcherLookup::self = nullptr;

class PatternMatchInfo {
  std::vector<int> groupMatchCounts;

public:
  PatternMatchInfo(int groupSize) : groupMatchCounts(groupSize) {
  }

  void setGroupMatchCount(int groupIndex, int count) {
    groupMatchCounts[groupIndex] = count;
  }

  int getGroupMatchCount(int groupIndex) {
    return groupMatchCounts[groupIndex];
  }

  int increment(int groupIndex) {
    setGroupMatchCount(groupIndex, getGroupMatchCount(groupIndex) + 1);
    return getGroupMatchCount(groupIndex);
  }
};

class AstTransformData {
  std::list<EnhancedToken*> tokens;
  PatternMatchInfo* patternMatchInfo;
public:
  void setTokens(std::list<EnhancedToken*> tokens) {
    this -> tokens = tokens;
  }
  std::list<EnhancedToken*>* getTokens() {
    return &tokens;
  }

  void setPatternMatchInfo(PatternMatchInfo* patternMatchInfo) {
    this -> patternMatchInfo = patternMatchInfo;
  }
  PatternMatchInfo* getPatternMatchInfo() {
    return patternMatchInfo;
  }
};

class AstTransform {
  std::function<AstNode*(AstTransformData*)> transformer;

public:
  AstTransform(std::function<AstNode*(AstTransformData*)> transformer) {
    this -> transformer = transformer;
  }

  AstNode* transform(AstTransformData* astTransformData) {
    return transformer(astTransformData);
  }
};

class AstTransformLookup {
  static AstTransformLookup *self;

  std::map<std::string, AstTransform*> astTransformHash;

  AstTransformLookup() {};
  AstTransformLookup(const AstTransformLookup& _) {}
  void operator=(const AstTransformLookup& _) {}
public:
  static AstTransformLookup* getInstance() {
    if (self == nullptr) {
      self = new AstTransformLookup();
    }

    return self;
  }

  void addAstTransform(std::string key, AstTransform* astTransform) {
    astTransformHash.insert({key, astTransform});
  }

  AstTransform* getAstTransform(std::string key) {
    #ifdef ECHELON_DEBUG
    if (astTransformHash.find(key) == astTransformHash.end()) {
      std::cout << key << "Missing ast transformer [" << key << "]";
      throw std::runtime_error("Request for missing ast transformer");
    }
    #endif

    return astTransformHash.at(key);
  }
};

AstTransformLookup* AstTransformLookup::self = nullptr;

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
  Matcher* matcher;

  bool subProcess = false;
public:
  TokenPatternElement(std::string element);

  std::string getData() const {
    return data;
  }

  Matcher* getMatcher() const {
    return matcher;
  }

  bool isSubProcess() {
    return subProcess;
  }
};

TokenPatternElement::TokenPatternElement(std::string element) {
  data = element;

  // Get the appropriate matcher.
  matcher = MatcherLookup::getInstance() -> getMatcher(data);

  if (data == "block") {
    subProcess = true;
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
  int getRepeatLowerBound() const {
    return repeatLowerBound;
  }

  void setRepeatUpperBound(int repeatUpperBound) {
    this -> repeatUpperBound = repeatUpperBound;
  }
  int getRepeatUpperBound() const {
    return repeatUpperBound;
  }
};

class TokenPattern {
  std::vector<TokenPatternGroup*> tokenPatternGroups;

  std::string id;
public:
  void addGroup(TokenPatternGroup* tokenPatternGroup) {
    tokenPatternGroups.push_back(tokenPatternGroup);
  }

  std::vector<TokenPatternGroup*>* getGroups() {
    return &tokenPatternGroups;
  }

  void setId(std::string id) {
    this -> id = id;
  }
  std::string getId() {
    return id;
  }
};

void stream_dump(std::ostream& s, TokenPatternElement* tokenPatternElement) {
  s << tokenPatternElement -> getData();
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

void stream_dump(std::ostream& s, EnhancedToken* enhancedToken) {
  s << enhancedToken -> getData() << ", " << EchelonLookup::getInstance() -> toString(enhancedToken -> getTokenType());
}

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
  TokenPattern *tokenPattern = new TokenPattern();

  for (auto i = pattern.begin(); i != pattern.end(); i++) {
    TokenPatternGroup *tokenPatternGroup = new TokenPatternGroup();

    if (*i == ' ') {
      continue;
    }
    else if (*i == '[') {
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

void stream_dump(std::ostream& s, AstNode* node, int level = 1) {
  s << "Level " << level << "\n";

  s << EchelonLookup::toString(node -> getType()) << ", ";
  s << node -> getData();
  s << "\n";

  for (int i = 0; i < node -> getChildCount(); i++) {
    stream_dump(s, node -> getChild(i), level + 1);
    s << "\n";
  }
}

class AstConstructionManager {
  AstNode *root;
public:
  AstConstructionManager() {
    root = new AstNode();
  }

  void pushFragment(AstNode* fragment) {

  }

  AstNode* getRoot() {
    return root;
  }
};

class Parser2 {
private:
  std::vector<TokenPattern*> tokenPatterns;

  AstNode* _parse(std::list<Token*>& tokens) {

    for (auto i = tokens.begin(); i != tokens.end(); i++) {

      std::cout << "Start processing at token "; stream_dump(std::cout, *i); std::cout << "\n";

      for (auto p = tokenPatterns.begin(); p != tokenPatterns.end(); p++) {
        bool patternMatches = true;

        std::cout << "Trying pattern "; stream_dump(std::cout, *p); std::cout << "\n";

        auto it = i;

        // Create a pattern match info to track group matches etc.
        PatternMatchInfo *patternMatchInfo = new PatternMatchInfo((*p) -> getGroups() -> size());

        // match each group in this pattern against the token.
        for (auto g = (*p) -> getGroups() -> begin(); g != (*p) -> getGroups() -> end(); g++) {

          std::cout << "Process group "; stream_dump(std::cout, *g); std::cout << "\n";

          auto itt = it;
          std::cout << "Current starting token "; stream_dump(std::cout, *itt); std::cout << "\n";

          int matchCount = 0;

          for (auto element = (*g) -> getElements() -> begin(); element != (*g) -> getElements() -> end(); element++) {
            std::cout << "_\n";
            EnhancedToken *enhancedToken = new EnhancedToken(*itt);

            std::cout << "Matches: {"; stream_dump(std::cout, enhancedToken); std::cout << "} ? ";

            if ((*element) -> isSubProcess()) {
              std::cout << "sub process";
              auto finishGroup = *(g.next());
              // call self, from current iterator position.
              // pass down the NEXT group from this one, then we can suppress "no patterns match" in the next level
              // down if the first non-matching token in the level below matches this next group.

              // this will involve extracting the group match below?
            }

            if ((*element) -> getMatcher() -> matches(enhancedToken)) {
              std::cout << "Yes\n" << std::endl;
              matchCount++;
              itt++;

              if (itt == tokens.end()) {
                std::cout << "Ran out of tokens.\n";
                break;
              }
            }
            else {
              std::cout << "No\n";
              break;
            }
          }

          bool completeGroupMatch = matchCount == (*g) -> getElements() -> size();
          std::cout << "Complete group match? " << toString(completeGroupMatch) << "\n";

          // doesn't match but is optional or repeating
          // does match and can't match again
          // does match and can match again

          if (completeGroupMatch) {
            int groupMatchCount = patternMatchInfo -> increment(std::distance((*p) -> getGroups() -> begin(), g));

            // Go again if we're under the uppper bound or are allowed unlimited matches.
            if (groupMatchCount < (*g) -> getRepeatUpperBound() || (*g) -> getRepeatUpperBound() == -1) {
              std::cout << "Match this group again.\n";
              g--; // repeat this group.
            }
            else {
              std::cout << "Finished with this group. Move on.\n";
              // matched but no need to repeat, therefore we just let the loop continue;
            }
          }
          else {
            // No match.

            int groupMatchCount = patternMatchInfo -> getGroupMatchCount(std::distance((*p) -> getGroups() -> begin(), g));

            if (groupMatchCount >= (*g) -> getRepeatLowerBound()) {
              // We've actually matched enough to allow the match even though this one failed.
              std::cout << "No group match, allowing anyway.\n";

              // However, we need to reset itt to give back the tokens we've used so far.
              itt = it;
            }
            else {
              patternMatches = false;
              break;
            }
          }

          // This group matches so we want to consume the tokens matched by this group.
          std::cout << "Consume " << std::distance(it, itt) << " tokens.\n";
          // need to be more careful than this?
          std::advance(it, std::distance(it, itt));
        }

        if (patternMatches) {
          std::cout << "Pattern matches.\n";

          // We've matched this whole pattern, so we want to consume tokens.
          std::cout << "Confirm consume " << std::distance(i, it) << " tokens\n";
          std::list<EnhancedToken*> matchedTokens;
          for (int k = 0; k < std::distance(i, it); k++) {
            matchedTokens.push_back(new EnhancedToken(*std::next(i, k)));
          }

          AstTransformData *td = new AstTransformData();
          td -> setPatternMatchInfo(patternMatchInfo);
          td -> setTokens(matchedTokens);

          auto transformer = AstTransformLookup::getInstance() -> getAstTransform((*p) -> getId());

          // TODO
          stream_dump(std::cout, transformer -> transform(td));

          std::advance(i, std::distance(i, it));
          break;
        }
      }

      if (i == tokens.end()) {
        break;
      }
    }
  }
public:
  AstNode* parse(std::list<Token*> tokens) {
    return _parse(tokens);
  }

  void addTokenPattern(TokenPattern* tokenPattern) {
    tokenPatterns.push_back(tokenPattern);
  }

  void addTokenPattern(std::string id, std::string tokenPattern) {
    static PatternTranslator patternTranslator;
    auto pattern = patternTranslator.translate(tokenPattern);
    pattern -> setId(id);
    tokenPatterns.push_back(pattern);
  }
};

int main(int argc, char** args) {
  if (Keyword::Integer == "integer") {
    std::cout << "wow.\n";
  }

  EchelonLookup::getInstance() -> addDataTypeKeyword("integer");
  EchelonLookup::getInstance() -> addDataTypeKeyword("string");

  EchelonLookup::getInstance() -> addKeyword("package");

  std::cout << EchelonLookup::toString(Keyword::Module) << std::endl;
  std::cout << EchelonLookup::toString(TokenTypeEnum::Identifier) << std::endl;

  Matcher *type = new Matcher();
  type -> setMatcher([&type] () -> bool {
    if (type -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance() -> isDataTypeKeyword(type -> getEnhancedToken() -> getData());
  });

  MatcherLookup::getInstance() -> addMatcher("type", type);

  Matcher *keyword = new Matcher();
  keyword -> setMatcher([&keyword] () -> bool {
    if (keyword -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance() -> isKeyword(keyword -> getEnhancedToken() -> getData());
  });

  MatcherLookup::getInstance() -> addMatcher("keyword", keyword);

  Matcher *identifier = new Matcher();
  identifier -> setMatcher([&identifier] () -> bool {
    // need to check not a keyword? or seperate matcher for that might be better.
    return identifier -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::Identifier;
  });

  MatcherLookup::getInstance() -> addMatcher("identifier", identifier);

  Matcher *kwd_package = new Matcher();
  kwd_package -> setMatcher([&kwd_package] () -> bool {
    if (kwd_package -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return kwd_package -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Package);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_package", kwd_package);

  Matcher *op_structure = new Matcher();
  op_structure -> setMatcher([&op_structure] () -> bool {
    return op_structure -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::StructureOperator;
  });

  MatcherLookup::getInstance() -> addMatcher("op_structure", op_structure);

  AstTransform *packageTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    AstNode *currentNode = base;

    auto builder = [&currentNode] (EnhancedToken* t) {
      currentNode -> setType(AstNodeType::Package);
      currentNode -> setData(t -> getData());

      AstNode *astNode = new AstNode();
      currentNode -> putChild(astNode);
      currentNode = astNode;
    };

    auto it = astTransformData -> getTokens() -> begin();
    it++; // skip the package keyword.
    for (int i = 0; i < astTransformData -> getPatternMatchInfo() -> getGroupMatchCount(1); i++) {
      builder(*it);

      it++;it++;
    }

    currentNode -> setType(AstNodeType::Package);
    currentNode -> setData((*it) -> getData());

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("package", packageTransform);

  // may contain the toString of any token type.
  std::string var_decl = "[type] identifier assign"; // should check non-kwd identifier.
  std::string assignment_expr = "[type] identifier assign expr";
  std::string for_loop = "kwd_for [type] identifier assign expr; bool_expr; expr block_delim_o [block] block_delim_c";
  std::string package = "kwd_package [identifier op_structure]* identifier";

  Parser2 p2;
  //p2.addTokenPattern(var_decl);
  //p2.addTokenPattern(assignment_expr);
  //p2.addTokenPattern(for_loop);
  p2.addTokenPattern("package", package);

  std::list<Token*> program;
  // package echelon::test_package
  program.push_back(new Token("package", TokenTypeEnum::Identifier));
  program.push_back(new Token("echelon", TokenTypeEnum::Identifier));
  program.push_back(new Token("::", TokenTypeEnum::StructureOperator));
  program.push_back(new Token("test_package", TokenTypeEnum::Identifier));

  p2.parse(program);

  std::list<Token*> program2;
  program2.push_back(new Token("package", TokenTypeEnum::Identifier));
  program2.push_back(new Token("test", TokenTypeEnum::Identifier));
  program2.push_back(new Token("::", TokenTypeEnum::StructureOperator));
  program2.push_back(new Token("pack", TokenTypeEnum::Identifier));
  program2.push_back(new Token("::", TokenTypeEnum::StructureOperator));
  program2.push_back(new Token("name", TokenTypeEnum::Identifier));

  p2.parse(program2);

  EnhancedToken *enhancedPackageKwd = new EnhancedToken(new Token("package", TokenTypeEnum::Identifier));
  std::cout << toString(keyword -> matches(enhancedPackageKwd)) << "\n";
  std::cout << toString(type -> matches(enhancedPackageKwd)) << "\n";

  return 0;
}
