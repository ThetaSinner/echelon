#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>

#include <echelon/parser/stage2/nested-pattern-lookup.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>
#include <echelon/parser/stage2/ast-transform.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>

void loadDataTypeKeywords() {
  EchelonLookup::getInstance() -> addDataTypeKeyword("integer");
  EchelonLookup::getInstance() -> addDataTypeKeyword("string");
}

void loadKeywords() {
  EchelonLookup::getInstance() -> addKeyword("package");
  EchelonLookup::getInstance() -> addKeyword("if");
}

void loadMatchers() {
  MatcherLookup::getInstance() -> addMatcher("type", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance() -> isDataTypeKeyword(self -> getEnhancedToken() -> getData());
  }));

  MatcherLookup::getInstance() -> addMatcher("keyword", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance() -> isKeyword(self -> getEnhancedToken() -> getData());
  }));

  MatcherLookup::getInstance() -> addMatcher("comment", Matcher::forTokenType(TokenType::SingleLineComment));
  MatcherLookup::getInstance() -> addMatcher("multi_line_comment", Matcher::forTokenType(TokenType::MultiLineComment));

  MatcherLookup::getInstance() -> addMatcher("identifier", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return !EchelonLookup::getInstance() -> isKeyword(self -> getEnhancedToken() -> getData());
  }));

  MatcherLookup::getInstance() -> addMatcher("kwd_package", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Package);
  }));

  MatcherLookup::getInstance() -> addMatcher("kwd_for", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::For);
  }));

  MatcherLookup::getInstance() -> addMatcher("kwd_module", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Module);
  }));

  MatcherLookup::getInstance() -> addMatcher("kwd_enum", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Enum);
  }));

  MatcherLookup::getInstance() -> addMatcher("op_structure", Matcher::forTokenType(TokenType::StructureOperator));
  MatcherLookup::getInstance() -> addMatcher("op_assign", Matcher::forTokenType(TokenType::Assign));
  MatcherLookup::getInstance() -> addMatcher("block_delim_o", Matcher::forTokenType(TokenType::BlockDelimO));
  MatcherLookup::getInstance() -> addMatcher("block_delim_c", Matcher::forTokenType(TokenType::BlockDelimC));
  MatcherLookup::getInstance() -> addMatcher("paren_open", Matcher::forTokenType(TokenType::ParenO));
  MatcherLookup::getInstance() -> addMatcher("paren_close", Matcher::forTokenType(TokenType::ParenC));
  // TODO remove, see op_comma.
  MatcherLookup::getInstance() -> addMatcher("list_seperator", Matcher::forTokenType(TokenType::CommaOperator));
  MatcherLookup::getInstance() -> addMatcher("add_operator", Matcher::forTokenType(TokenType::OperatorAdd));
  MatcherLookup::getInstance() -> addMatcher("subtract_operator", Matcher::forTokenType(TokenType::OperatorSubtract));
  MatcherLookup::getInstance() -> addMatcher("op_comma", Matcher::forTokenType(TokenType::CommaOperator));
  MatcherLookup::getInstance() -> addMatcher("multiply_operator", Matcher::forTokenType(TokenType::OperatorMultiply));
  MatcherLookup::getInstance() -> addMatcher("divide_operator", Matcher::forTokenType(TokenType::OperatorDivide));
  MatcherLookup::getInstance() -> addMatcher("string", Matcher::forTokenType(TokenType::String));
  MatcherLookup::getInstance() -> addMatcher("integer", Matcher::forTokenType(TokenType::Integer));
  MatcherLookup::getInstance() -> addMatcher("float", Matcher::forTokenType(TokenType::Float));

  MatcherLookup::getInstance() -> addMatcher("boolean", new Matcher([] (Matcher* self) -> bool {
    return
            self -> getEnhancedToken() -> getData() == EchelonLookup::toString(Keyword::True) ||
            self -> getEnhancedToken() -> getData() == EchelonLookup::toString(Keyword::False);
  }));

  MatcherLookup::getInstance() -> addMatcher("op_equality", Matcher::forTokenType(TokenType::Equality));
  MatcherLookup::getInstance() -> addMatcher("op_and", Matcher::forTokenType(TokenType::AndOperator));
  MatcherLookup::getInstance() -> addMatcher("op_or", Matcher::forTokenType(TokenType::OrOperator));

  MatcherLookup::getInstance() -> addMatcher("kwd_if", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::If);
  }));

  MatcherLookup::getInstance() -> addMatcher("kwd_else", new Matcher([] (Matcher* self) -> bool {
    if (self -> getEnhancedToken() -> getTokenType() != TokenType::Identifier) {
      return false;
    }

    return self -> getEnhancedToken() -> getData() == EchelonLookup::getInstance() -> toString(Keyword::Else);
  }));

  // TODO this is untidy, do I really need this?
  MatcherLookup::getInstance() -> addMatcher("block", new Matcher([] (Matcher* self) -> bool {
    throw std::runtime_error("Cannot match block directly.");
  }));
}

void loadTransformers() {
  AstTransformLookup::getInstance() -> addAstTransform("package", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
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
  }));

  AstTransformLookup::getInstance() -> addAstTransform("module", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Module);
    base -> setData((*std::next(astTransformData -> getTokens() -> begin(), 1)) -> getData());

    // think I'm creating an extra level that I don't need.
    if (!astTransformData -> getSubProcessAstNodes() -> empty()) {
      // Map all children of the sub process node as children of "base".
      for (unsigned i = 0; i < astTransformData -> getSubProcessAstNodes() -> front() -> getChildCount(); i++) {
        base -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(i));
      }
      astTransformData -> getSubProcessAstNodes() -> pop();
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("add", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::BinaryOperator);
    base -> setData(astTransformData -> getTokens() -> front() -> getData());

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("op_or", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::BooleanBinaryOperator);
    // TODO Map sub type.
    base -> setData(astTransformData -> getTokens() -> front() -> getData());

    // TODO astTransformData -> getNestedAstNodes();

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("function_call", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::FunctionCall);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    auto nested = astTransformData -> getNestedAstNodes();
    if (nested != nullptr && nested -> size()) {
      auto callParams = nested -> front() -> getChild(0);
      base -> putChild(callParams);
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("string", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
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
  }));

  AstTransformLookup::getInstance() -> addAstTransform("integer", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Integer);
    base -> setData(astTransformData -> getTokens() -> front() -> getData());

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
  }));

  AstTransformLookup::getInstance() -> addAstTransform("float", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Float);
    base -> setData(astTransformData -> getTokens() -> front() -> getData());

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
  }));

  AstTransformLookup::getInstance() -> addAstTransform("boolean", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Boolean);
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
  }));

  AstTransformLookup::getInstance() -> addAstTransform("expr_function_call", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    // Start with the function call node.
    AstNode *base = new AstNode();

    auto nested = astTransformData -> getNestedAstNodes();
    if (nested != nullptr && nested -> size() == 3) {
      auto left = nested -> front() -> getChild(0);
      nested -> pop();
      base = nested -> front() -> getChild(0);
      nested -> pop();
      auto right = nested -> front() -> getChild(0);
      nested -> pop();

      base -> putChild(left);
      base -> putChild(right);
    }
    else {
      base -> setType(AstNodeType::FunctionCall);
      base -> setData(astTransformData -> getNestedAstNodes() -> front() -> getChild(0) -> getData());
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("op_equality", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::EqualityOperator);
    base -> setData((*(astTransformData -> getTokens() -> begin())) -> getData());

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("bool_expr_val", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    // This transform is just a wrapper around a "expr" type, so just map the result from the "expr" transform.
    auto node = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    return node;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("bool_expr_compare", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    auto left = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    auto op = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    auto right = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();

    AstNode *base = new AstNode();
    base -> setType(AstNodeType::EqualityOperator);
    base -> setData(op -> getChild(0) -> getData()); // this is redundant

    base -> putChild(left -> getChild(0));
    base -> putChild(right -> getChild(0));

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("bool_expr_logic", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    auto left = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    auto op = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();
    auto right = astTransformData -> getNestedAstNodes() -> front();
    astTransformData -> getNestedAstNodes() -> pop();

    AstNode *base = new AstNode();
    base -> setType(AstNodeType::BooleanBinaryOperator);
    base -> setData(op -> getChild(0) -> getData()); // this is redundant

    base -> putChild(left -> getChild(0));
    base -> putChild(right -> getChild(0));

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("if_stmt", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::If);

    // Map the condition.
    base -> putChild(astTransformData -> getNestedAstNodes() -> front() -> getChild(0));
    astTransformData -> getNestedAstNodes() -> pop();

    // Map the block.
    // The block will always exist but may be empty.
    if (!astTransformData -> getSubProcessAstNodes() -> empty() && astTransformData -> getSubProcessAstNodes() -> front() -> getChildCount() > 0) {
      auto block = new AstNode();
      block -> setType(AstNodeType::Block);
      block -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(0));
      astTransformData -> getSubProcessAstNodes() -> pop();

      base -> putChild(block);
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("else_if_stmt", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::ElseIf);

    // Map the condition.
    base -> putChild(astTransformData -> getNestedAstNodes() -> front() -> getChild(0));
    astTransformData -> getNestedAstNodes() -> pop();

    // Map the block.
    // The block will always exist but may be empty.
    if (!astTransformData -> getSubProcessAstNodes() -> empty() && astTransformData -> getSubProcessAstNodes() -> front() -> getChildCount() > 0) {
      auto block = new AstNode();
      block -> setType(AstNodeType::Block);
      block -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(0));
      astTransformData -> getSubProcessAstNodes() -> pop();

      base -> putChild(block);
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("else_stmt", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Else);

    // The block will always exist but may be empty.
    if (!astTransformData -> getSubProcessAstNodes() -> empty() && astTransformData -> getSubProcessAstNodes() -> front() -> getChildCount() > 0) {
      auto block = new AstNode();
      block -> setType(AstNodeType::Block);
      block -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(0));
      astTransformData -> getSubProcessAstNodes() -> pop();

      base -> putChild(block);
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("if", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Branches);

    auto nested = astTransformData -> getNestedAstNodes();
    while (!nested -> empty()) {
      if (nested -> front() -> getChildCount() >= 1) {
        base -> putChild(nested -> front() -> getChild(0));
        nested -> pop();
      }
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("assignment_expr", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
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
  }));

  AstTransformLookup::getInstance() -> addAstTransform("var_decl", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
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

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("signature_item_term", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::FunctionParamDefinitions);

    auto paramDef = new AstNode();
    paramDef -> setType(AstNodeType::FunctionParamDefinition);
    if (astTransformData -> getTokens() -> size() > 0) {
      if (astTransformData -> getPatternMatchInfo() -> getGroupMatchCount(0) == 0) {
        paramDef -> setData(astTransformData -> getTokens() -> front() -> getData());
      }
      else {
        auto iter = astTransformData -> getTokens() -> begin();

        auto typeNode = new AstNode();
        typeNode -> setType(AstNodeType::Type);
        typeNode -> setData((*iter) -> getData());
        paramDef -> putChild(typeNode);

        iter++;
        paramDef -> setData((*iter) -> getData());
      }
    }
    base -> putChild(paramDef);

    while (!astTransformData -> getNestedAstNodes() -> empty()) {
      auto paramDefs = astTransformData -> getNestedAstNodes() -> back() -> getChild(0);
      for (unsigned i = 0; i < paramDefs -> getChildCount(); i++) {
        base -> putChild(paramDefs -> getChild(i));
      }
      astTransformData -> getNestedAstNodes() -> pop();
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("function_call_params", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::FunctionCallParams);

    // Map the new function call param.
    auto nestedAstNodes = astTransformData -> getNestedAstNodes();
    if (!nestedAstNodes -> empty()) {
      auto callParam = new AstNode();
      callParam -> setType(AstNodeType::FunctionCallParam);
      callParam -> putChild(nestedAstNodes -> front() -> getChild(0));
      base -> putChild(callParam);
      nestedAstNodes -> pop();
    }

    while (!nestedAstNodes -> empty()) {
      auto callParams = nestedAstNodes -> front() -> getChild(0);
      for (int i = 0; i < callParams -> getChildCount(); i++) {
        base -> putChild(callParams -> getChild(i));
      }
      nestedAstNodes -> pop();
    }

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("comment", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::SingleLineComment);
    base -> setData(astTransformData -> getTokens() -> front() -> getData());

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("multi_line_comment", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::MultiLineComment);
    base -> setData(astTransformData -> getTokens() -> front() -> getData());

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("function_call_stmt", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    return astTransformData -> getNestedAstNodes() -> front() -> getChild(0);
  }));

  AstTransformLookup::getInstance() -> addAstTransform("function", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    AstNode *base = new AstNode();
    base -> setType(AstNodeType::Function);

    if (astTransformData -> getPatternMatchInfo() -> getGroupMatchCount(0) == 0) {
      // return type not specified, first token is function name.
      base -> setData(astTransformData -> getTokens() -> front() -> getData());
    }
    else {
      auto iter = astTransformData -> getTokens() -> begin();

      auto typeNode = new AstNode();
      typeNode -> setType(AstNodeType::Type);
      typeNode -> setData((*iter) -> getData());
      base -> putChild(typeNode);

      iter++;
      base -> setData((*iter) -> getData());
    }

    if (!astTransformData -> getNestedAstNodes() -> empty()) {
      base -> putChild(astTransformData -> getNestedAstNodes() -> back() -> getChild(0));
      astTransformData -> getNestedAstNodes() -> pop();
    }

    // map the code block.
    auto block = new AstNode();
    block -> setType(AstNodeType::Block);
    block -> putChild(astTransformData -> getSubProcessAstNodes() -> front() -> getChild(0));
    base -> putChild(block);

    return base;
  }));

  AstTransformLookup::getInstance() -> addAstTransform("expr_statement", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    return astTransformData->getNestedAstNodes()->front()->getChild(0);
  }));

  AstTransformLookup::getInstance()->addAstTransform("enum", new AstTransform([] (AstTransformData* astTransformData) -> AstNode* {
    auto base = new AstNode();
    base->setType(AstNodeType::Enum);

    auto tokenIterator = astTransformData->getTokens()->begin();
    tokenIterator++;

    base->setData((*tokenIterator)->getData());

    tokenIterator++; tokenIterator++;

    auto enumConstants = new AstNode();
    enumConstants->setType(AstNodeType::EnumConstants);
    base->putChild(enumConstants);

    while ((*tokenIterator)->getTokenType() != TokenType::BlockDelimC) {
      auto enumConst = new AstNode();
      enumConst->setType(AstNodeType::EnumConstant);
      enumConst->setData((*tokenIterator)->getData());
      enumConstants->putChild(enumConst);
      tokenIterator++;
    }

    return base;
  }));
}

void loadNested() {
  std::string binaryOperator = "binary_operator";
  NestedPatternLookup::getInstance() -> registerNested(
          binaryOperator,
          "add",
          "add_operator");
  NestedPatternLookup::getInstance() -> registerNested(
          binaryOperator,
          "subtract",
          "subtract_operator");
  NestedPatternLookup::getInstance() -> registerNested(
          binaryOperator,
          "multiply",
          "multiply_operator");
  NestedPatternLookup::getInstance() -> registerNested(
          binaryOperator,
          "divide",
          "divide_operator");

  NestedPatternLookup::getInstance() -> forwardDeclareNested("function_call");
  std::string expr = "expr";
  NestedPatternLookup::getInstance() -> registerNested(
          expr,
          "expr_function_call",
          "function_call [binary_operator expr]");
  NestedPatternLookup::getInstance() -> registerNested(
          expr,
          "string",
          "string [binary_operator expr]");
  NestedPatternLookup::getInstance() -> registerNested(
          expr,
          "integer",
          "integer [binary_operator expr]");
  NestedPatternLookup::getInstance() -> registerNested(
          expr,
          "float",
          "float [binary_operator expr]");
  NestedPatternLookup::getInstance() -> registerNested(
          expr,
          "boolean",
          "boolean [binary_operator expr]");

  std::string any_compare_op = "any_compare_op";
  NestedPatternLookup::getInstance() -> registerNested(
          any_compare_op,
          "op_equality",
          "op_equality");

  std::string any_logic_op = "any_logic_op";
  NestedPatternLookup::getInstance() -> registerNested(
          any_logic_op,
          "op_and",
          "op_and");
  NestedPatternLookup::getInstance() -> registerNested(
          any_logic_op,
          "op_or",
          "op_or");

  std::string bool_expr = "bool_expr";
  NestedPatternLookup::getInstance() -> registerNested(
          bool_expr,
          "bool_expr_compare",
          "expr any_compare_op bool_expr");
  NestedPatternLookup::getInstance() -> registerNested(
          bool_expr,
          "bool_expr_logic",
          "expr any_logic_op bool_expr");
  // Important this is loaded last
  NestedPatternLookup::getInstance() -> registerNested(
          bool_expr,
          "bool_expr_val",
          "expr");

  NestedPatternLookup::getInstance() -> registerNested(
          "if_stmt",
          "if_stmt",
          "kwd_if paren_open bool_expr paren_close block_delim_o [block] block_delim_c");

  NestedPatternLookup::getInstance() -> registerNested(
          "else_if_stmt",
          "else_if_stmt",
          "kwd_else kwd_if paren_open bool_expr paren_close block_delim_o [block] block_delim_c");

  NestedPatternLookup::getInstance() -> registerNested(
          "else_stmt",
          "else_stmt",
          "kwd_else block_delim_o [block] block_delim_c");

  NestedPatternLookup::getInstance() -> registerNested(
          "signature_item",
          "signature_item_term",
          "[type] identifier [op_comma signature_item]");

  NestedPatternLookup::getInstance() -> registerNested(
          "function_call_params",
          "function_call_params",
          "expr [op_comma function_call_params]"
  );
  NestedPatternLookup::getInstance() -> registerNested(
          "function_call",
          "function_call",
          "identifier paren_open [function_call_params] paren_close"
  );
}

void loadPatterns() {
  TokenPatternLookup::getInstance() -> addTokenPattern(
          "function",
          "[type] identifier paren_open [signature_item] paren_close block_delim_o [block] block_delim_c");

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "function_call_stmt",
          "function_call"
  );

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "assignment_expr",
          "[type] identifier op_assign expr");

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "var_decl",
          "type identifier");

  std::string for_loop = "kwd_for [type] identifier op_assign expr; bool_expr; expr block_delim_o [block] block_delim_c";

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "package",
          "kwd_package [identifier op_structure]* identifier");

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "module",
          "kwd_module identifier block_delim_o [block] block_delim_c");

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "if",
          "if_stmt [else_if_stmt]* [else_stmt]");

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "comment",
          "comment");

  TokenPatternLookup::getInstance() -> addTokenPattern(
          "multi_line_comment",
          "multi_line_comment");

  TokenPatternLookup::getInstance() -> addTokenPattern(
      "expr_statement",
      "expr"
  );

  TokenPatternLookup::getInstance() -> addTokenPattern(
      "enum",
      "kwd_enum identifier block_delim_o [identifier]* block_delim_c"
  );
}

void loadParserStage2Data() {
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
