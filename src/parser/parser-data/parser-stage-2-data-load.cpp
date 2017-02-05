#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>

#include <echelon/parser/stage2/nested-pattern-lookup.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>
#include <echelon/parser/stage2/ast-transform.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/keyword-enum.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/to-string.hpp>
#include <echelon/parser/parser-data/parser-stage-2-helper.hpp>
#include <echelon/util/enum-class-iterator.hpp>

void loadDataTypeKeywords() {
  /* TODO
   * This has to be manually maintained.
   *
   * C++ Doesn't have the flexibility with enum classes for what is required here.
   */

  EchelonLookup::getInstance()->addDataTypeKeyword(EchelonLookup::getInstance()->toString(Keyword::Integer));
  EchelonLookup::getInstance()->addDataTypeKeyword(EchelonLookup::getInstance()->toString(Keyword::String));
}

void loadKeywords() {
  for (auto i : Enum<Keyword>()) {
    EchelonLookup::getInstance()->addKeyword(EchelonLookup::getInstance()->toString(i));
  }
}

void loadMatchers() {
  MatcherLookup::getInstance()->addMatcher("comment", Matcher::forTokenType(TokenType::SingleLineComment));
  MatcherLookup::getInstance()->addMatcher("multi_line_comment", Matcher::forTokenType(TokenType::MultiLineComment));

  MatcherLookup::getInstance()->addMatcher("kwd_package", Matcher::forKeyword(Keyword::Package));
  MatcherLookup::getInstance()->addMatcher("kwd_module", Matcher::forKeyword(Keyword::Module));
  MatcherLookup::getInstance()->addMatcher("kwd_enum", Matcher::forKeyword(Keyword::Enum));
  MatcherLookup::getInstance()->addMatcher("kwd_until", Matcher::forKeyword(Keyword::Until));
  MatcherLookup::getInstance()->addMatcher("kwd_while", Matcher::forKeyword(Keyword::While));
  MatcherLookup::getInstance()->addMatcher("kwd_if", Matcher::forKeyword(Keyword::If));
  MatcherLookup::getInstance()->addMatcher("kwd_else", Matcher::forKeyword(Keyword::Else));
  MatcherLookup::getInstance()->addMatcher("kwd_behaviour", Matcher::forKeyword(Keyword::Behaviour));
  MatcherLookup::getInstance()->addMatcher("kwd_function", Matcher::forKeyword(Keyword::Function));
  MatcherLookup::getInstance()->addMatcher("kwd_each", Matcher::forKeyword(Keyword::Each));
  MatcherLookup::getInstance()->addMatcher("kwd_in", Matcher::forKeyword(Keyword::In));
  MatcherLookup::getInstance()->addMatcher("kwd_type", Matcher::forKeyword(Keyword::Type));
  MatcherLookup::getInstance()->addMatcher("kwd_public", Matcher::forKeyword(Keyword::Public));

  MatcherLookup::getInstance()->addMatcher("block_delimiter_open", Matcher::forTokenType(TokenType::BlockDelimiterOpen));
  MatcherLookup::getInstance()->addMatcher("block_delimiter_close", Matcher::forTokenType(TokenType::BlockDelimiterClose));

  MatcherLookup::getInstance()->addMatcher("parenthesis_open", Matcher::forTokenType(TokenType::ParenthesisOpen));
  MatcherLookup::getInstance()->addMatcher("parenthesis_close", Matcher::forTokenType(TokenType::ParenthesisClose));

  MatcherLookup::getInstance()->addMatcher("add_operator", Matcher::forTokenType(TokenType::AddOperator));
  MatcherLookup::getInstance()->addMatcher("subtract_operator", Matcher::forTokenType(TokenType::SubtractOperator));
  MatcherLookup::getInstance()->addMatcher("multiply_operator", Matcher::forTokenType(TokenType::MultiplyOperator));
  MatcherLookup::getInstance()->addMatcher("divide_operator", Matcher::forTokenType(TokenType::DivideOperator));

  MatcherLookup::getInstance()->addMatcher("integer", Matcher::forTokenType(TokenType::Integer));
  MatcherLookup::getInstance()->addMatcher("string", Matcher::forTokenType(TokenType::String));
  MatcherLookup::getInstance()->addMatcher("decimal", Matcher::forTokenType(TokenType::Decimal));

  MatcherLookup::getInstance()->addMatcher("boolean_value_false", Matcher::forKeyword(Keyword::False));
  MatcherLookup::getInstance()->addMatcher("boolean_value_true", Matcher::forKeyword(Keyword::True));

  MatcherLookup::getInstance()->addMatcher("logical_negation", Matcher::forTokenType(TokenType::LogicalNegation));
  MatcherLookup::getInstance()->addMatcher("logical_equality", Matcher::forTokenType(TokenType::LogicalEquality));
  MatcherLookup::getInstance()->addMatcher("logical_conjunction", Matcher::forTokenType(TokenType::LogicalConjunction));
  MatcherLookup::getInstance()->addMatcher("logical_disjunction", Matcher::forTokenType(TokenType::LogicalDisjunction));

  MatcherLookup::getInstance()->addMatcher("structure_operator", Matcher::forTokenType(TokenType::StructureOperator));
  MatcherLookup::getInstance()->addMatcher("assignment", Matcher::forTokenType(TokenType::Assignment));
  MatcherLookup::getInstance()->addMatcher("comma", Matcher::forTokenType(TokenType::Comma));
  MatcherLookup::getInstance()->addMatcher("forward_arrow_operator", Matcher::forTokenType(TokenType::ForwardArrowOperator));
  MatcherLookup::getInstance()->addMatcher("ellipsis_operator", Matcher::forTokenType(TokenType::EllipsisOperator));

  MatcherLookup::getInstance()->addMatcher("type_name", new Matcher([](Matcher *self) -> bool {
    if (self->getEnhancedToken()->getTokenType() != TokenType::Identifier) {
      return false;
    }

    return EchelonLookup::getInstance()->isDataTypeKeyword(self->getEnhancedToken()->getData());
  }));

  MatcherLookup::getInstance()->addMatcher("identifier", new Matcher([](Matcher *self) -> bool {
    if (self->getEnhancedToken()->getTokenType() != TokenType::Identifier) {
      return false;
    }

    // TODO this is an error in the source code. Needs to be reported. Unfortunately it's not that simple, the matcher may be called during a pattern match which ultimately fails.
    return !EchelonLookup::getInstance()->isKeyword(self->getEnhancedToken()->getData());
  }));

  // TODO this is untidy, do I really need this?
  MatcherLookup::getInstance()->addMatcher("block", new Matcher([](Matcher *self) -> bool {
    throw std::runtime_error("Cannot match block directly.");
  }));
}

void loadTransformers() {
  AstTransformLookup::getInstance()->addAstTransform("package", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        AstNode *currentNode = base;

        auto builder = [&currentNode](EnhancedToken *t) {
          currentNode->setType(AstNodeType::Package);
          currentNode->setData(t->getData());

          AstNode *astNode = new AstNode();
          currentNode->putChild(astNode);
          currentNode = astNode;
        };

        auto it = astTransformData->getTokens()->begin();
        it++; // skip the package keyword.
        for (int i = 0; i < astTransformData->getPatternMatchInfo()->getGroupMatchCount(1); i++) {
          builder(*it);

          it++;
          it++;
        }

        currentNode->setType(AstNodeType::Package);
        currentNode->setData((*it)->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("module", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::Module);
        base->setData((*std::next(astTransformData->getTokens()->begin(), 1))->getData());

        // think I'm creating an extra level that I don't need.
        if (!astTransformData->getSubProcessAstNodes()->empty()) {
          auto block = new AstNode();
          block->setType(AstNodeType::Block);
          base->putChild(block);

          // Map all children of the sub process node as children of "base".
          for (unsigned i = 0; i < astTransformData->getSubProcessAstNodes()->front()->getChildCount(); i++) {
            block->putChild(astTransformData->getSubProcessAstNodes()->front()->getChild(i));
          }
          astTransformData->getSubProcessAstNodes()->pop();
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("add_operator", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::BinaryOperatorAdd);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("subtract_operator", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::BinaryOperatorSubtract);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("multiply_operator", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::BinaryOperatorMultiply);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("divide_operator", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::BinaryOperatorDivide);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("logical_disjunction", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::LogicalDisjunction);
        base->setData(astTransformData->getTokens()->front()->getData()); // redundant.

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("logical_conjunction", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::LogicalConjunction);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("function_call", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::FunctionCall);
        base->setData(astTransformData->getTokens()->front()->getData()); // function name to call.

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size()) {
          auto callParams = nested->front()->getChild(0);
          base->putChild(callParams);
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("string", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::String);
        base->setData(astTransformData->getTokens()->front()->getData()); // string data.

        // TODO extract expression builder code.
        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size() == 2) {
          auto oper = nested->front();
          nested->pop();
          auto nextExpr = nested->front();
          nested->pop();

          oper->getChild(0)->putChild(base);
          oper->getChild(0)->putChild(nextExpr->getChild(0));
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("expression_variable", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        // [subtract_operator] [name_structure] identifier [binary_operator expression]

        AstNode *base = new AstNode();

        auto nested = astTransformData->getNestedAstNodes();
        int nameStructureDepth = 0;
        if (nested != nullptr && !nested->empty() && nested->front()->getChild(0)->getType() == AstNodeType::NameStructure) {
          auto nameStructure = nested->front()->getChild(0);
          base->putChild(nameStructure);

          auto nextNested = nameStructure;
          nameStructureDepth++;
          while (nextNested->hasChild(AstNodeType::NameStructure)) {
            nextNested = nextNested->getChild(AstNodeType::NameStructure);
            nameStructureDepth++;
          }

          nested->pop();
        }

        auto tokens = astTransformData->getTokens();
        if (tokens->front()->getTokenType() == TokenType::SubtractOperator) {
          base->setType(AstNodeType::UnaryMinus);
          tokens->pop_front();

          for (int i = 0; i < nameStructureDepth; i++) { tokens->pop_front(); tokens->pop_front(); }

          auto variable = new AstNode();
          variable->setType(AstNodeType::VariableValue);
          variable->setData(tokens->front()->getData());
          tokens->pop_front();

          base->putChild(variable);
        }
        else {
          for (int i = 0; i < nameStructureDepth; i++) { tokens->pop_front(); tokens->pop_front(); }

          base->setType(AstNodeType::VariableValue);
          base->setData(tokens->front()->getData());
          tokens->pop_front();
        }

        if (nested != nullptr && nested->size() == 2) {
          auto oper = nested->front();
          nested->pop();
          auto nextExpr = nested->front();
          nested->pop();

          oper->getChild(0)->putChild(base);
          oper->getChild(0)->putChild(nextExpr->getChild(0));
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("integer", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        std::list<EnhancedToken *> *tokens = astTransformData->getTokens();
        if (tokens->front()->getTokenType() == TokenType::SubtractOperator) {
          base->setType(AstNodeType::UnaryMinus);
          tokens->pop_front();

          auto integer = new AstNode();
          integer->setType(AstNodeType::Integer);
          integer->setData(tokens->front()->getData());
          tokens->pop_front();
          
          base->putChild(integer);
        }
        else {
          base->setType(AstNodeType::Integer);
          base->setData(tokens->front()->getData());
          tokens->pop_front();
        }

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size() == 2) {
          auto oper = nested->front();
          nested->pop();
          auto nextExpr = nested->front();
          nested->pop();

          oper->getChild(0)->putChild(base);
          oper->getChild(0)->putChild(nextExpr->getChild(0));
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("decimal", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::Decimal);
        base->setData(astTransformData->getTokens()->front()->getData());

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size() == 2) {
          auto oper = nested->front();
          nested->pop();
          auto nextExpr = nested->front();
          nested->pop();

          oper->getChild(0)->putChild(base);
          oper->getChild(0)->putChild(nextExpr->getChild(0));
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("boolean_value_true", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::BooleanTrue);
        base->setData(astTransformData->getTokens()->front()->getData()); // redundant

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size() == 2) {
          auto oper = nested->front();
          nested->pop();
          auto nextExpr = nested->front();
          nested->pop();

          oper->getChild(0)->putChild(base);
          oper->getChild(0)->putChild(nextExpr->getChild(0));
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("boolean_value_false", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::BooleanFalse);
        base->setData(astTransformData->getTokens()->front()->getData()); // redundant

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size() == 2) {
          auto oper = nested->front();
          nested->pop();
          auto nextExpr = nested->front();
          nested->pop();

          oper->getChild(0)->putChild(base);
          oper->getChild(0)->putChild(nextExpr->getChild(0));
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("expression_function_call", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        // Start with the function call node.
        AstNode *base = new AstNode();

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && nested->size() == 3) {
          auto left = nested->front()->getChild(0);
          nested->pop();
          base = nested->front()->getChild(0);
          nested->pop();
          auto right = nested->front()->getChild(0);
          nested->pop();

          base->putChild(left);
          base->putChild(right);
        } else {
          base->setType(AstNodeType::FunctionCall);
          base->setData(astTransformData->getNestedAstNodes()->front()->getChild(0)->getData());
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("logical_equality", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::EqualityOperator);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("boolean_expression_value", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base;

        if (astTransformData->getTokens()->front()->getTokenType() == TokenType::LogicalNegation) {
          base = new AstNode();
          base->setType(AstNodeType::LogicalNegation);
          base->putChild(astTransformData->getNestedAstNodes()->front()->getChild(0));
        } else {
          base = astTransformData->getNestedAstNodes()->front()->getChild(0);
        }

        astTransformData->getNestedAstNodes()->pop();

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("boolean_expression_compare", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto left = astTransformData->getNestedAstNodes()->front();
        astTransformData->getNestedAstNodes()->pop();
        auto op = astTransformData->getNestedAstNodes()->front();
        astTransformData->getNestedAstNodes()->pop();
        auto right = astTransformData->getNestedAstNodes()->front();
        astTransformData->getNestedAstNodes()->pop();

        AstNode *base = op->getChild(0);
        base->putChild(left->getChild(0));
        base->putChild(right->getChild(0));

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("boolean_expression_logic", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto left = astTransformData->getNestedAstNodes()->front();
        astTransformData->getNestedAstNodes()->pop();
        auto op = astTransformData->getNestedAstNodes()->front();
        astTransformData->getNestedAstNodes()->pop();
        auto right = astTransformData->getNestedAstNodes()->front();
        astTransformData->getNestedAstNodes()->pop();

        AstNode *base = op->getChild(0);

        if (astTransformData->getTokens()->front()->getTokenType() == TokenType::LogicalNegation) {
          auto invert_left = new AstNode();
          invert_left->setType(AstNodeType::LogicalNegation);
          invert_left->putChild(left->getChild(0));

          base->putChild(invert_left);
        } else {
          base->putChild(left->getChild(0));
        }

        base->putChild(right->getChild(0));

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("if_statement", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::If);

        // Map the condition.
        base->putChild(astTransformData->getNestedAstNodes()->front()->getChild(0));
        astTransformData->getNestedAstNodes()->pop();

        // Map the block.
        // The block will always exist but may be empty.
        if (!astTransformData->getSubProcessAstNodes()->empty() &&
            astTransformData->getSubProcessAstNodes()->front()->getChildCount() > 0) {
          auto block = new AstNode();
          block->setType(AstNodeType::Block);
          block->putChild(astTransformData->getSubProcessAstNodes()->front()->getChild(0));
          astTransformData->getSubProcessAstNodes()->pop();

          base->putChild(block);
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("else_if_statement", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::ElseIf);

        // Map the condition.
        base->putChild(astTransformData->getNestedAstNodes()->front()->getChild(0));
        astTransformData->getNestedAstNodes()->pop();

        // Map the block.
        // The block will always exist but may be empty.
        if (!astTransformData->getSubProcessAstNodes()->empty() &&
            astTransformData->getSubProcessAstNodes()->front()->getChildCount() > 0) {
          auto block = new AstNode();
          block->setType(AstNodeType::Block);
          block->putChild(astTransformData->getSubProcessAstNodes()->front()->getChild(0));
          astTransformData->getSubProcessAstNodes()->pop();

          base->putChild(block);
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("else_statement", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::Else);

        // The block will always exist but may be empty.
        if (!astTransformData->getSubProcessAstNodes()->empty() &&
            astTransformData->getSubProcessAstNodes()->front()->getChildCount() > 0) {
          auto block = new AstNode();
          block->setType(AstNodeType::Block);
          block->putChild(astTransformData->getSubProcessAstNodes()->front()->getChild(0));
          astTransformData->getSubProcessAstNodes()->pop();

          base->putChild(block);
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("if", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::Branches);

        auto nested = astTransformData->getNestedAstNodes();
        while (!nested->empty()) {
          if (nested->front()->getChildCount() >= 1) {
            base->putChild(nested->front()->getChild(0));
            nested->pop();
          }
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("assignment_expression", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto tokenIterator = astTransformData->getTokens()->begin();

        AstNode *base = new AstNode();
        base->setType(AstNodeType::Variable);

        if (!(*tokenIterator)->isDataTypeKeyword()) {
          base->setData((*tokenIterator)->getData());
        } else {
          AstNode *type = new AstNode();
          type->setType(AstNodeType::TypeName);
          type->setData((*tokenIterator)->getData());
          base->putChild(type);

          tokenIterator++;
          base->setData((*tokenIterator)->getData());
        }

        auto nested = astTransformData->getNestedAstNodes();
        if (nested != nullptr && !nested->empty()) {
          auto expressionNode = new AstNode();
          expressionNode->setType(AstNodeType::Expression);
          expressionNode->putChild(nested->front()->getChild(0));

          base->putChild(expressionNode);
        }

        return base;
      }));


  AstTransformLookup::getInstance()->addAstTransform("variable_declaration", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto tokenIterator = astTransformData->getTokens()->begin();

        AstNode *base = new AstNode();
        base->setType(AstNodeType::Variable);

        if (!(*tokenIterator)->isDataTypeKeyword()) {
          base->setData((*tokenIterator)->getData());
        } else {
          AstNode *type = new AstNode();
          type->setType(AstNodeType::TypeName);
          type->setData((*tokenIterator)->getData());
          base->putChild(type);

          tokenIterator++;
          base->setData((*tokenIterator)->getData());
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("parameter_definition", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::FunctionParamDefinitions);

        auto paramDef = new AstNode();
        paramDef->setType(AstNodeType::FunctionParamDefinition);
        if (astTransformData->getTokens()->size() > 0) {
          if (astTransformData->getPatternMatchInfo()->getGroupMatchCount(0) == 0) {
            paramDef->setData(astTransformData->getTokens()->front()->getData());
          } else {
            auto iter = astTransformData->getTokens()->begin();

            auto typeNode = new AstNode();
            typeNode->setType(AstNodeType::TypeName);
            typeNode->setData((*iter)->getData());
            paramDef->putChild(typeNode);

            iter++;
            paramDef->setData((*iter)->getData());
          }
        }
        base->putChild(paramDef);

        while (!astTransformData->getNestedAstNodes()->empty()) {
          auto paramDefs = astTransformData->getNestedAstNodes()->back()->getChild(0);
          for (unsigned i = 0; i < paramDefs->getChildCount(); i++) {
            base->putChild(paramDefs->getChild(i));
          }
          astTransformData->getNestedAstNodes()->pop();
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("function_call_params", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::FunctionCallParams);

        // Map the new function call param.
        auto nestedAstNodes = astTransformData->getNestedAstNodes();
        if (!nestedAstNodes->empty()) {
          auto callParam = new AstNode();
          callParam->setType(AstNodeType::FunctionCallParam);
          callParam->putChild(nestedAstNodes->front()->getChild(0));
          base->putChild(callParam);
          nestedAstNodes->pop();
        }

        while (!nestedAstNodes->empty()) {
          auto callParams = nestedAstNodes->front()->getChild(0);
          for (unsigned i = 0; i < callParams->getChildCount(); i++) {
            base->putChild(callParams->getChild(i));
          }
          nestedAstNodes->pop();
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("comment", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::SingleLineComment);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("multi_line_comment", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::MultiLineComment);
        base->setData(astTransformData->getTokens()->front()->getData());

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("top_level_function_call", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        return astTransformData->getNestedAstNodes()->front()->getChild(0);
      }));

  AstTransformLookup::getInstance()->addAstTransform("function", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::Function);

        auto nested = astTransformData->getNestedAstNodes();
        // Map the name structure.
        int nameStructureDepth = 0;
        if (!nested->empty() && nested->front()->getChild(0)->getType() == AstNodeType::NameStructure) {
          auto nameStructure = nested->front()->getChild(0);
          base->putChild(nameStructure);

          auto nsRecurse = nameStructure;
          nameStructureDepth++;
          while (nsRecurse->hasChild(AstNodeType::NameStructure)) {
            nsRecurse = nsRecurse->getChild(AstNodeType::NameStructure);
            nameStructureDepth++;
          }
        }
        // Map the parameter definitions.
        if (!nested->empty()) {
          base->putChild(nested->back()->getChild(0));
          nested->pop();
        }
        
        // Map the function name.
        auto iter = astTransformData->getTokens()->begin();
        iter++; // skip the function keyword.
        // TODO this is the first time this has been necessary, but it's nasty. Would be better if tokens matched by nested patterns weren't passed forward to here.
        for (int i = 0; i < nameStructureDepth; i++) { iter++; iter++; } // skip the name structure
        base->setData((*iter)->getData());

        while (iter != astTransformData->getTokens()->end() &&
               (*iter)->getTokenType() != TokenType::ForwardArrowOperator &&
               (*iter)->getTokenType() != TokenType::BlockDelimiterOpen) {
          iter++;
        }

        // This isn't the best condition. But if we aren't pointing at a block open, then assume we're looking at a return type.
        if ((*iter)->getTokenType() != TokenType::BlockDelimiterOpen) {
          iter++;

          // Map the return type.
          auto typeNode = new AstNode();
          typeNode->setType(AstNodeType::TypeName);
          typeNode->setData((*iter)->getData());
          base->putChild(typeNode);
        }

        // map the code block.
        auto subProcess = astTransformData->getSubProcessAstNodes();
        if (!subProcess->empty()) {
          auto block = new AstNode();
          block->setType(AstNodeType::Block);
          block->putChild(subProcess->front()->getChild(0));
          base->putChild(block);
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("function_prototype", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        AstNode *base = new AstNode();
        base->setType(AstNodeType::Function);

        auto nested = astTransformData->getNestedAstNodes();
        // Map the access specifier.
        bool hasAccessSpecifier = false;
        if (!nested->empty() && nested->front()->getChild(0)->getType() == AstNodeType::AccessSpecification) {
          hasAccessSpecifier = true;
          base->putChild(nested->front()->getChild(0));
          nested->pop();
        }
        // Map the parameter definitions.
        if (!nested->empty()) {
          base->putChild(nested->back()->getChild(0));
          nested->pop();
        }

        // Map the function name.
        auto iter = astTransformData->getTokens()->begin();
        if (hasAccessSpecifier) iter++; // skip the access specifier.
        iter++; // skip the function keyword.
        base->setData((*iter)->getData());

        while (iter != astTransformData->getTokens()->end() &&
               (*iter)->getTokenType() != TokenType::ForwardArrowOperator &&
               (*iter)->getTokenType() != TokenType::BlockDelimiterOpen) {
          iter++;
        }

        iter++;
        // TODO check that iter actually points at a return type. This involves allowing and recognising more complicated return types though.
        // Map the return type.
        auto typeNode = new AstNode();
        typeNode->setType(AstNodeType::TypeName);
        typeNode->setData((*iter)->getData());
        base->putChild(typeNode);

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("top_level_expression", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        return astTransformData->getNestedAstNodes()->front()->getChild(0);
      }));

  AstTransformLookup::getInstance()->addAstTransform("top_level_boolean_expression", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        return astTransformData->getNestedAstNodes()->front()->getChild(0);
      }));

  AstTransformLookup::getInstance()->addAstTransform("parenthesis_expression", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto nested = astTransformData->getNestedAstNodes();

        LoggerSharedInstance::get()->at(Level::Debug) << to_string(astTransformData->getTokens()) << "\n";

        auto base = new AstNode();
        if (astTransformData->getTokens()->front()->getTokenType() == TokenType::SubtractOperator) {
          base->setType(AstNodeType::UnaryMinus);

          auto expr_group = new AstNode();
          expr_group->setType(AstNodeType::ExprGroup);
          expr_group->putChild(nested->front()->getChild(0));
          nested->pop();

          base->putChild(expr_group);
        }
        else {
          base->setType(AstNodeType::ExprGroup);
          base->putChild(nested->front()->getChild(0));
          nested->pop();
        }

        if (nested->size()) {
          auto oper = nested->front()->getChild(0);
          nested->pop();

          auto next_expr = nested->front()->getChild(0);
          nested->pop();

          oper->putChild(base);
          oper->putChild(next_expr);
          base = oper;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("enum", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::Enum);

        auto tokenIterator = astTransformData->getTokens()->begin();
        tokenIterator++;

        base->setData((*tokenIterator)->getData());

        tokenIterator++;
        tokenIterator++;

        auto enumConstants = new AstNode();
        enumConstants->setType(AstNodeType::EnumConstants);
        base->putChild(enumConstants);

        // TODO if other things are allowed in an enum block then this isn't a good condition.
        while ((*tokenIterator)->getTokenType() != TokenType::BlockDelimiterClose) {
          auto enumConst = new AstNode();
          enumConst->setType(AstNodeType::EnumConstant);
          enumConst->setData((*tokenIterator)->getData());
          enumConstants->putChild(enumConst);
          tokenIterator++;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("until", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::Until);

        auto condition = new AstNode();
        condition->setType(AstNodeType::Condition);
        condition->putChild(astTransformData->getNestedAstNodes()->front()->getChild(0));
        base->putChild(condition);

        ParserStage2Helper::mapSubProcessAsBlock(astTransformData, base);

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("while", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::While);

        auto condition = new AstNode();
        condition->setType(AstNodeType::Condition);
        condition->putChild(astTransformData->getNestedAstNodes()->front()->getChild(0));
        base->putChild(condition);

        ParserStage2Helper::mapSubProcessAsBlock(astTransformData, base);

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("behaviour", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::Behaviour);

        auto iter = astTransformData->getTokens()->begin();
        iter++;
        base->setData((*iter)->getData());

        ParserStage2Helper::mapSubProcessAsBlock(astTransformData, base);

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("type", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::Type);

        auto iter = astTransformData->getTokens()->begin();
        iter++;
        base->setData((*iter)->getData());

        ParserStage2Helper::mapSubProcessAsBlock(astTransformData, base);

        return base;
      }));

  // TODO check overwrite when ast transformer is added.

  AstTransformLookup::getInstance()->addAstTransform("each", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::Each);

        auto iter = astTransformData->getTokens()->begin();
        iter++; // skip the each keyword.

        auto iterator = new AstNode();
        iterator->setType(AstNodeType::Iterator);
        iterator->setData((*iter)->getData());
        base->putChild(iterator);

        // Map the expression_range
        iterator->putChild(astTransformData->getNestedAstNodes()->front()->getChild(0));

        ParserStage2Helper::mapSubProcessAsBlock(astTransformData, base);

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("expression_range", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::ExpressionRange);

        auto nested = astTransformData->getNestedAstNodes();

        auto beginNode = new AstNode();
        beginNode->setType(AstNodeType::ExpressionRangeBegin);
        beginNode->putChildFront(nested->front()->getChild(0));
        base->putChild(beginNode);

        nested->pop();

        auto endNode = new AstNode();
        endNode->setType(AstNodeType::ExpressionRangeEnd);
        endNode->putChild(nested->front()->getChild(0));
        base->putChild(endNode);

        nested->pop();

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("parenthesis_boolean_expression", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::ExprGroup);

        auto bool_expr = astTransformData->getNestedAstNodes()->front()->getChild(0);
        base->putChild(bool_expr);

        if (astTransformData->getTokens()->front()->getTokenType() == TokenType::LogicalNegation) {
          auto invert = new AstNode();
          invert->setType(AstNodeType::LogicalNegation);
          invert->putChild(base);

          base = invert;
        }

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("access_specifier_public", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::AccessSpecification);

        auto _public = new AstNode();
        _public->setType(AstNodeType::AccessSpecifierPublic);
        base->putChild(_public);

        return base;
      }));

  AstTransformLookup::getInstance()->addAstTransform("name_structure", new AstTransform(
      [](AstTransformData *astTransformData) -> AstNode * {
        auto base = new AstNode();
        base->setType(AstNodeType::NameStructure);
        LoggerSharedInstance::get()->at(Level::Info) << to_string(astTransformData->getTokens()) << "\n";
        base->setData(astTransformData->getTokens()->front()->getData());

        auto nested = astTransformData->getNestedAstNodes();
        if (nested->size()) {
          base->putChild(nested->front()->getChild(0));
        }

        return base;
      }));
}

void loadNested() {
  std::string binaryOperator = "binary_operator";
  NestedPatternLookup::getInstance()->registerNested(
      binaryOperator,
      "add_operator",
      "add_operator"
  );
  NestedPatternLookup::getInstance()->registerNested(
      binaryOperator,
      "subtract_operator",
      "subtract_operator"
  );
  NestedPatternLookup::getInstance()->registerNested(
      binaryOperator,
      "multiply_operator",
      "multiply_operator"
  );
  NestedPatternLookup::getInstance()->registerNested(
      binaryOperator,
      "divide_operator",
      "divide_operator"
  );

  NestedPatternLookup::getInstance()->forwardDeclareNested("function_call");
  NestedPatternLookup::getInstance()->forwardDeclareNested("name_structure");
  std::string expression = "expression";
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "parenthesis_expression",
      "[subtract_operator] parenthesis_open expression parenthesis_close [binary_operator expression]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "expression_function_call",
      "function_call [binary_operator expression]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "string",
      "string [binary_operator expression]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "integer",
      "[subtract_operator] integer [binary_operator expression]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "decimal",
      "decimal [binary_operator expression]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "boolean_value_false",
      "boolean_value_false [binary_operator expression]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "boolean_value_true",
      "boolean_value_true [binary_operator expression]"
  );
  // TODO this should be moved up but true and false are recognised as identifiers.. see keyword todo above.
  NestedPatternLookup::getInstance()->registerNested(
      expression,
      "expression_variable",
      "[subtract_operator] [name_structure] identifier [binary_operator expression]"
  );

  std::string any_comparison_operator = "any_comparison_operator";
  NestedPatternLookup::getInstance()->registerNested(
      any_comparison_operator,
      "logical_equality",
      "logical_equality"
  );

  std::string any_logical_operator = "any_logical_operator";
  NestedPatternLookup::getInstance()->registerNested(
      any_logical_operator,
      "logical_conjunction",
      "logical_conjunction"
  );
  NestedPatternLookup::getInstance()->registerNested(
      any_logical_operator,
      "logical_disjunction",
      "logical_disjunction"
  );

  std::string boolean_expression = "boolean_expression";
  NestedPatternLookup::getInstance()->registerNested(
      boolean_expression,
      "parenthesis_boolean_expression",
      "[logical_negation] parenthesis_open boolean_expression parenthesis_close"
  );
  NestedPatternLookup::getInstance()->registerNested(
      boolean_expression,
      "boolean_expression_compare",
      "expression any_comparison_operator boolean_expression"
  );
  NestedPatternLookup::getInstance()->registerNested(
      boolean_expression,
      "boolean_expression_logic",
      "[logical_negation] expression any_logical_operator boolean_expression"
  );
  // Important this is loaded last
  NestedPatternLookup::getInstance()->registerNested(
      boolean_expression,
      "boolean_expression_value",
      "[logical_negation] expression"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "if_statement",
      "if_statement",
      "kwd_if parenthesis_open boolean_expression parenthesis_close block_delimiter_open [block] block_delimiter_close"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "else_if_statement",
      "else_if_statement",
      "kwd_else kwd_if parenthesis_open boolean_expression parenthesis_close block_delimiter_open [block] block_delimiter_close"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "else_statement",
      "else_statement",
      "kwd_else block_delimiter_open [block] block_delimiter_close"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "parameter_definitions",
      "parameter_definition",
      "[type_name] identifier [comma parameter_definitions]"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "function_call_params",
      "function_call_params",
      "expression [comma function_call_params]"
  );
  NestedPatternLookup::getInstance()->registerNested(
      "function_call",
      "function_call",
      "identifier parenthesis_open [function_call_params] parenthesis_close"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "iterable",
      "expression_range",
      "expression ellipsis_operator expression" // TODO the expressions need to evaluate to something which can be stepped through. e.g. integers.
  );

  auto access_specifier = "access_specifier";
  NestedPatternLookup::getInstance()->registerNested(
      access_specifier,
      "access_specifier_public",
      "kwd_public"
  );

  NestedPatternLookup::getInstance()->registerNested(
      "name_structure",
      "name_structure",
      "identifier structure_operator [name_structure]"
  );
}

/* TODO use nested for everything and default the parser to using the "default" group at top level.
 * That way custom scopes can be defined which contain top level elements. For example class methods.
 *
 * Or by adding a scope list to the top level lookup, otherwise the nested behaviour becomes insanely complex.
 */
void loadPatterns() {
  TokenPatternLookup::getInstance()->addTokenPattern(
      "function",
      "kwd_function [name_structure] identifier parenthesis_open [parameter_definitions] parenthesis_close [forward_arrow_operator type_name] block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "function_prototype",
      "[access_specifier] kwd_function identifier parenthesis_open [parameter_definitions] parenthesis_close [forward_arrow_operator type_name]"
  );

  // TODO this is covered by expressions.
  TokenPatternLookup::getInstance()->addTokenPattern(
      "top_level_function_call",
      "function_call"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "assignment_expression",
      "[type_name] identifier assignment expression"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "variable_declaration",
      "type_name identifier"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "package",
      "kwd_package [identifier structure_operator]* identifier"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "module",
      "kwd_module identifier block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "if",
      "if_statement [else_if_statement]* [else_statement]"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "comment",
      "comment"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "multi_line_comment",
      "multi_line_comment"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "enum",
      "kwd_enum identifier block_delimiter_open [identifier]* block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "until",
      "kwd_until parenthesis_open boolean_expression parenthesis_close block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "while",
      "kwd_while parenthesis_open boolean_expression parenthesis_close block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "behaviour",
      "kwd_behaviour identifier block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "type",
      "kwd_type identifier block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "each",
      "kwd_each identifier kwd_in iterable block_delimiter_open [block] block_delimiter_close"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "top_level_expression",
      "expression"
  );

  TokenPatternLookup::getInstance()->addTokenPattern(
      "top_level_boolean_expression",
      "boolean_expression"
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
