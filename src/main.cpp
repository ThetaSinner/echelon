#include <iostream>
#include <string>
#include <set>
#include <list>
#include <algorithm>
#include <vector>
#include <stack>
#include <functional>
#include <iterator>
#include <queue>

#include <echelon/parser/tokenizer.hpp>
#include <echelon/parser/token.hpp>
#include <echelon/parser/token-type-enum.hpp>

#include <echelon/ast/AstNode.hpp>
#include <echelon/ast/AstNodeType.hpp>

#define ECHELON_DEBUG

#include <echelon/parser/stage2/echelon-lookup.hpp>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/parser/stage2/matcher.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>
#include <echelon/parser/stage2/pattern-match-info.hpp>
#include <echelon/parser/stage2/ast-transform-data.hpp>
#include <echelon/parser/stage2/ast-transform.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/parser/stage2/token-pattern-element.hpp>
#include <echelon/parser/stage2/token-pattern-group.hpp>
#include <echelon/parser/stage2/token-pattern.hpp>
#include <echelon/util/iterator-util.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>
#include <echelon/parser/stage2/ast-construction-manager.hpp>
#include <echelon/parser/stage2/parser-internal-input.hpp>
#include <echelon/parser/stage2/parser-internal-output.hpp>
#include <echelon/parser/stage2/parser.hpp>

#include <echelon/util/stream-dump.hpp>

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

  Matcher *kwd_for = new Matcher();
  kwd_for -> setMatcher([&kwd_for] () -> bool {
    if (kwd_for -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return kwd_for -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::For);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_for", kwd_for);

  Matcher *kwd_module = new Matcher();
  kwd_module -> setMatcher([&kwd_module] () -> bool {
    if (kwd_module -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return kwd_module -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Module);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_module", kwd_module);

  Matcher *op_structure = new Matcher();
  op_structure -> setMatcher([&op_structure] () -> bool {
    return op_structure -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::StructureOperator;
  });

  MatcherLookup::getInstance() -> addMatcher("op_structure", op_structure);

  Matcher *op_assign = new Matcher();
  op_assign -> setMatcher([&op_assign] () -> bool {
    return op_assign -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::Assign;
  });

  MatcherLookup::getInstance() -> addMatcher("op_assign", op_assign);

  Matcher *block_delim_o = new Matcher();
  block_delim_o -> setMatcher([&block_delim_o] () -> bool {
    return block_delim_o -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::BlockDelimO;
  });

  MatcherLookup::getInstance() -> addMatcher("block_delim_o", block_delim_o);

  Matcher *block_delim_c = new Matcher();
  block_delim_c -> setMatcher([&block_delim_c] () -> bool {
    return block_delim_c -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::BlockDelimC;
  });

  MatcherLookup::getInstance() -> addMatcher("block_delim_c", block_delim_c);

  Matcher *block = new Matcher();
  block -> setMatcher([&block] () -> bool {
    throw std::runtime_error("Cannot match block directly.");
  });

  MatcherLookup::getInstance() -> addMatcher("block", block);

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

  AstTransform *moduleTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Module);
    base -> setData((*std::next(astTransformData -> getTokens() -> begin(), 1)) -> getData());

    // think I'm creating an extra level that I don't need.
    if (!astTransformData -> getSubProcessAstNodes() -> empty()) {
      // Map all children of the sub process node as children of "base".
      for (int i = 0; i < astTransformData -> getSubProcessAstNodes() -> front() -> getChildCount(); i++) {
        base -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(i));
      }
      astTransformData -> getSubProcessAstNodes() -> pop();
    }

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("module", moduleTransform);

  // may contain the toString of any token type.
  std::string var_decl = "[type] identifier assign"; // should check non-kwd identifier.
  std::string assignment_expr = "[type] identifier assign expr";
  std::string for_loop = "kwd_for [type] identifier op_assign expr; bool_expr; expr block_delim_o [block] block_delim_c";
  std::string package = "kwd_package [identifier op_structure]* identifier";
  std::string module = "kwd_module identifier block_delim_o [block] block_delim_c";

  Parser2 p2;
  //p2.addTokenPattern(var_decl);
  //p2.addTokenPattern(assignment_expr);
  //p2.addTokenPattern("for_loop", for_loop);
  p2.addTokenPattern("module", module);
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

  std::list<Token*> program3;

  program3.push_back(new Token("module", TokenTypeEnum::Identifier));
  program3.push_back(new Token("name", TokenTypeEnum::Identifier));
  program3.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program3.push_back(new Token("module", TokenTypeEnum::Identifier));
  program3.push_back(new Token("nested", TokenTypeEnum::Identifier));
  program3.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  // no content yet.
  program3.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program3.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  p2.parse(program3);

  std::list<Token*> program4;

  program4.push_back(new Token("module", TokenTypeEnum::Identifier));
  program4.push_back(new Token("name", TokenTypeEnum::Identifier));
  program4.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program4.push_back(new Token("module", TokenTypeEnum::Identifier));
  program4.push_back(new Token("nested1", TokenTypeEnum::Identifier));
  program4.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program4.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program4.push_back(new Token("module", TokenTypeEnum::Identifier));
  program4.push_back(new Token("nested2", TokenTypeEnum::Identifier));
  program4.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program4.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program4.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  p2.parse(program4);

  EnhancedToken *enhancedPackageKwd = new EnhancedToken(new Token("package", TokenTypeEnum::Identifier));
  std::cout << toString(keyword -> matches(enhancedPackageKwd)) << "\n";
  std::cout << toString(type -> matches(enhancedPackageKwd)) << "\n";

  return 0;
}
