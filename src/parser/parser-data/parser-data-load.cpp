#include <echelon/parser/parser-data/parser-data-load.hpp>

#include <echelon/parser/stage2/echelon-lookup.hpp>
#include <echelon/parser/stage2/matcher.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>
#include <echelon/parser/stage2/ast-transform.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>

void loadDataTypeKeywords() {
  EchelonLookup::getInstance() -> addDataTypeKeyword("integer");
  EchelonLookup::getInstance() -> addDataTypeKeyword("string");
}

void loadKeywords() {
  EchelonLookup::getInstance() -> addKeyword("package");
}

void loadMatchers() {
  Matcher *type = new Matcher();
  type -> setMatcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance() -> isDataTypeKeyword(self -> getEnhancedToken() -> getData());
  });

  MatcherLookup::getInstance() -> addMatcher("type", type);

  Matcher *keyword = new Matcher();
  keyword -> setMatcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance() -> isKeyword(self -> getEnhancedToken() -> getData());
  });

  MatcherLookup::getInstance() -> addMatcher("keyword", keyword);

  Matcher *identifier = new Matcher();
  identifier -> setMatcher([] (Matcher* self) -> bool {
    // need to check not a keyword? or seperate matcher for that might be better.
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::Identifier;
  });

  MatcherLookup::getInstance() -> addMatcher("identifier", identifier);

  Matcher *kwd_package = new Matcher();
  kwd_package -> setMatcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Package);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_package", kwd_package);

  Matcher *kwd_for = new Matcher();
  kwd_for -> setMatcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::For);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_for", kwd_for);

  Matcher *kwd_module = new Matcher();
  kwd_module -> setMatcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Module);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_module", kwd_module);

  Matcher *op_structure = new Matcher();
  op_structure -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::StructureOperator;
  });

  MatcherLookup::getInstance() -> addMatcher("op_structure", op_structure);

  Matcher *op_assign = new Matcher();
  op_assign -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::Assign;
  });

  MatcherLookup::getInstance() -> addMatcher("op_assign", op_assign);

  Matcher *block_delim_o = new Matcher();
  block_delim_o -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::BlockDelimO;
  });

  MatcherLookup::getInstance() -> addMatcher("block_delim_o", block_delim_o);

  Matcher *block_delim_c = new Matcher();
  block_delim_c -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::BlockDelimC;
  });

  MatcherLookup::getInstance() -> addMatcher("block_delim_c", block_delim_c);

  Matcher *block = new Matcher();
  block -> setMatcher([] (Matcher* self) -> bool {
    throw std::runtime_error("Cannot match block directly.");
  });

  MatcherLookup::getInstance() -> addMatcher("block", block);
}

void loadTransformers() {
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
}

void loadPatterns() {
  std::string var_decl = "[type] identifier assign"; // should check non-kwd identifier.
  std::string assignment_expr = "[type] identifier assign expr";
  std::string for_loop = "kwd_for [type] identifier op_assign expr; bool_expr; expr block_delim_o [block] block_delim_c";

  std::string package = "kwd_package [identifier op_structure]* identifier";
  TokenPatternLookup::getInstance() -> addTokenPattern("package", package);

  std::string module = "kwd_module identifier block_delim_o [block] block_delim_c";
  TokenPatternLookup::getInstance() -> addTokenPattern("module", module);
}

void loadParserData() {
  static bool loaded = false;

  if (!loaded) {
    loadDataTypeKeywords();
    loadKeywords();
    loadMatchers();
    loadPatterns();
    loadTransformers();
    loaded = true;
  }
}
