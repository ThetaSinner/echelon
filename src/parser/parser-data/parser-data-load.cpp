#include <echelon/parser/parser-data/parser-data-load.hpp>

#include <echelon/parser/stage2/nested-pattern-lookup.hpp>
#include <echelon/parser/stage2/echelon-lookup.hpp>
#include <echelon/parser/stage2/matcher.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>
#include <echelon/parser/stage2/ast-transform.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <echelon/util/stream-dump.hpp>
#endif

void loadDataTypeKeywords() {
  EchelonLookup::getInstance() -> addDataTypeKeyword("integer");
  EchelonLookup::getInstance() -> addDataTypeKeyword("string");
}

void loadKeywords() {
  EchelonLookup::getInstance() -> addKeyword("package");
  EchelonLookup::getInstance() -> addKeyword("if");
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

  Matcher *paren_open = new Matcher();
  paren_open -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::ParenO;
  });

  MatcherLookup::getInstance() -> addMatcher("paren_open", paren_open);

  Matcher *paren_close = new Matcher();
  paren_close -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::ParenC;
  });

  MatcherLookup::getInstance() -> addMatcher("paren_close", paren_close);

  Matcher *list_seperator = new Matcher();
  list_seperator -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getData() == ",";
  });

  MatcherLookup::getInstance() -> addMatcher("list_seperator", list_seperator);

  Matcher *add_operator = new Matcher();
  add_operator -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getData() == "+";
  });

  MatcherLookup::getInstance() -> addMatcher("add_operator", add_operator);

  Matcher *subtract_operator = new Matcher();
  subtract_operator -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getData() == "-";
  });

  MatcherLookup::getInstance() -> addMatcher("subtract_operator", subtract_operator);

  Matcher *multiply_operator = new Matcher();
  multiply_operator -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getData() == "*";
  });

  MatcherLookup::getInstance() -> addMatcher("multiply_operator", multiply_operator);

  Matcher *divide_operator = new Matcher();
  divide_operator -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getData() == "/";
  });

  MatcherLookup::getInstance() -> addMatcher("divide_operator", divide_operator);

  Matcher *string = new Matcher();
  string -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::String;
  });

  MatcherLookup::getInstance() -> addMatcher("string", string);

  Matcher *integer = new Matcher();
  integer -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::Integer;
  });

  MatcherLookup::getInstance() -> addMatcher("integer", integer);

  Matcher *op_equality = new Matcher();
  op_equality -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::Equality;
  });

  MatcherLookup::getInstance() -> addMatcher("op_equality", op_equality);

  Matcher *op_and = new Matcher();
  op_and -> setMatcher([] (Matcher* self) -> bool {
    return self -> getEnhancedToken() -> getTokenType() == TokenTypeEnum::AndOperator;
  });

  MatcherLookup::getInstance() -> addMatcher("op_and", op_and);

  Matcher *kwd_if = new Matcher();
  kwd_if -> setMatcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenTypeEnum::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::If);
  });

  MatcherLookup::getInstance() -> addMatcher("kwd_if", kwd_if);


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

  AstTransform *addTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::BinaryOperator);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    #ifdef ECHELON_DEBUG
    //std::cout << "Build add "; stream_dump(std::cout, base); std::cout << std::endl;
    #endif

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("add", addTransform);

  AstTransform *functionCallTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    // TODO deal with params to function call.

    AstNode *base = new AstNode();
    base -> setType(AstNodeType::FunctionCall);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    auto nested = astTransformData -> getNestedAstNodes();
    #ifdef ECHELON_DEBUG
    stream_dump(std::cout, nested);
    #endif
    if (nested != nullptr && nested -> size() == 2) {
      auto oper = nested -> front();
      nested -> pop();
      auto nextExpr = nested -> front();
      nested -> pop();

      oper -> getChild(0) -> putChild(base);
      oper -> getChild(0) -> putChild(nextExpr -> getChild(0));
      base = oper;
    }

    #ifdef ECHELON_DEBUG
    //std::cout << "Build function call: "; stream_dump(std::cout, base); std::cout << std::endl;
    #endif

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("function_call", functionCallTransform);

  AstTransform *exprStringTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::String);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    auto nested = astTransformData -> getNestedAstNodes();
    if (nested != nullptr && nested -> size() == 2) {
      auto oper = nested -> front();
      nested -> pop();
      auto nextExpr = nested -> front();
      nested -> pop();

      oper -> getChild(0) -> putChild(base);
      oper -> getChild(0) -> putChild(nextExpr -> getChild(0));
      base = oper;
    }

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("string", exprStringTransform);

  AstTransform *exprIntegerTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Integer);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    auto nested = astTransformData -> getNestedAstNodes();
    if (nested != nullptr && nested -> size() == 2) {
      auto oper = nested -> front();
      nested -> pop();
      auto nextExpr = nested -> front();
      nested -> pop();

      oper -> getChild(0) -> putChild(base);
      oper -> getChild(0) -> putChild(nextExpr -> getChild(0));
      base = oper;
    }

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("integer", exprIntegerTransform);

  AstTransform *assignOperatorTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::EqualityOperator);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    /*
    auto nested = astTransformData -> getNestedAstNodes();
    if (nested != nullptr && nested -> size() == 2) {
      auto oper = nested -> front();
      nested -> pop();
      auto nextExpr = nested -> front();
      nested -> pop();

      oper -> getChild(0) -> putChild(base);
      oper -> getChild(0) -> putChild(nextExpr -> getChild(0));
      base = oper;
    }
    */

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("op_equality", assignOperatorTransform);

  AstTransform *boolExpressionValueTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    // This transform is just a wrapper around a "expr" type, so just map the result from the "expr" transform.
    auto node = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    return node;
  });

  AstTransformLookup::getInstance() -> addAstTransform("bool_expr_val", boolExpressionValueTransform);

  AstTransform *boolExpressionCompareTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    auto left = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    auto op = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    auto right = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();

    AstNode *base = new AstNode();
    base -> setType(AstNodeType::EqualityOperator);
    base -> setData(op -> getData()); // this is redundant

    base -> putChild(left -> getChild(0));
    base -> putChild(right -> getChild(0));

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("bool_expr_compare", boolExpressionCompareTransform);

  AstTransform *ifTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::If);

    base -> putChild(astTransformData -> getNestedAstNodes() -> front() -> getChild(0));
    astTransformData -> getNestedAstNodes() -> pop();
    // The block will always exist but may be empty.
    if (!astTransformData -> getSubProcessAstNodes() -> empty() && astTransformData -> getSubProcessAstNodes() -> front() -> getChildCount() > 0) {
      base -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(0));
      astTransformData -> getSubProcessAstNodes() -> pop();
    }

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("if", ifTransform);

  AstTransform *assignmentExprTransform = new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    auto tokenIterator = astTransformData -> getTokens() -> begin();

    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Variable);

    if (!(*tokenIterator) -> isDataTypeKeyword()) {
      base -> setData((*tokenIterator) -> getData());
    }
    else {
      AstNode *type = new AstNode();
      type -> setType(AstNodeType::Type);
      type -> setData((*tokenIterator) -> getData());
      base -> putChild(type);

      tokenIterator++;
      base -> setData((*tokenIterator) -> getData());
    }

    auto nested = astTransformData -> getNestedAstNodes();
    if (nested != nullptr && !nested -> empty()) {
      base -> putChild(nested -> front() -> getChild(0));
    }

    return base;
  });

  AstTransformLookup::getInstance() -> addAstTransform("assignment_expr", assignmentExprTransform);
}

void loadNested() {
  std::string binaryOperator = "binary_operator";

  std::string add = "add_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, "add", add);
  std::string subtract = "subtract_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, "subtract", subtract);
  std::string multiply = "multiply_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, "multiply", multiply);
  std::string divide = "divide_operator";
  NestedPatternLookup::getInstance() -> registerNested(binaryOperator, "divide", divide);

  std::string expr = "expr";

  std::string function_call = "identifier paren_open [expr list_seperator]* [expr] paren_close [binary_operator expr]";
  NestedPatternLookup::getInstance() -> registerNested(expr, "function_call", function_call);
  std::string expr_string = "string [binary_operator expr]";
  NestedPatternLookup::getInstance() -> registerNested(expr, "string", expr_string);
  std::string expr_integer = "integer [binary_operator expr]";
  NestedPatternLookup::getInstance() -> registerNested(expr, "integer", expr_integer);

  std::string any_compare_op = "any_compare_op";

  std::string op_equality = "op_equality";
  NestedPatternLookup::getInstance() -> registerNested(any_compare_op, "op_equality", op_equality);

  std::string any_logic_op = "any_logic_op";

  std::string op_and = "op_and";
  NestedPatternLookup::getInstance() -> registerNested(any_logic_op, "op_and", op_and);

  std::string bool_expr = "bool_expr";

  std::string bool_expr_compare = "expr any_compare_op bool_expr";
  NestedPatternLookup::getInstance() -> registerNested(bool_expr, "bool_expr_compare", bool_expr_compare);
  std::string bool_expr_logic = "expr any_logic_op bool_expr";
  NestedPatternLookup::getInstance() -> registerNested(bool_expr, "bool_expr_logic", bool_expr_logic);
  // Important this is loaded last
  std::string bool_expr_val = "expr";
  NestedPatternLookup::getInstance() -> registerNested(bool_expr, "bool_expr_val", bool_expr_val);
}

void loadPatterns() {
  std::string var_decl = "[type] identifier assign"; // should check non-kwd identifier.
  std::string assignment_expr = "[type] identifier op_assign expr";
  TokenPatternLookup::getInstance() -> addTokenPattern("assignment_expr", assignment_expr);
  std::string for_loop = "kwd_for [type] identifier op_assign expr; bool_expr; expr block_delim_o [block] block_delim_c";

  std::string package = "kwd_package [identifier op_structure]* identifier";
  TokenPatternLookup::getInstance() -> addTokenPattern("package", package);

  std::string module = "kwd_module identifier block_delim_o [block] block_delim_c";
  TokenPatternLookup::getInstance() -> addTokenPattern("module", module);

  std::string _if = "kwd_if paren_open bool_expr paren_close block_delim_o [block] block_delim_c";
  TokenPatternLookup::getInstance() -> addTokenPattern("if", _if);
}

void loadParserData() {
  static bool loaded = false;

  if (!loaded) {
    loadDataTypeKeywords();
    loadKeywords();
    loadMatchers();
    loadNested();
    loadPatterns();
    loadTransformers();
    loaded = true;
  }
}
