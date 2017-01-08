#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>

#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/stage1/character-matcher-lookup.hpp>
#include <echelon/parser/stage1/character-pattern-lookup.hpp>
#include <echelon/parser/stage1/character-pattern-parser.hpp>

void loadCharacterMatchers();

void loadCharacterPatterns();

void loadParserStage1Data() {
  static bool loaded = false;

  if (!loaded) {
    loadCharacterMatchers();
    loadCharacterPatterns();
  }

  loaded = true;
}

void loadCharacterMatchers() {
  CharacterMatcherLookup::getInstance()->addCharacterMatcher("number", [](char c) -> bool {
    return '0' <= c && c <= '9';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("letter", [](char c) -> bool {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("underscore", [](char c) -> bool {
    return c == '_';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("full_stop", [](char c) -> bool {
    return c == '.';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("double_quote", [](char c) -> bool {
    return c == '\"';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("forward_slash", [](char c) -> bool {
    return c == '/';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("new_line", [](char c) -> bool {
    return c == '\n';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("star", [](char c) -> bool {
    return c == '*';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("plus", [](char c) -> bool {
    return c == '+';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("ampersand", [](char c) -> bool {
    return c == '&';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("pipe", [](char c) -> bool {
    return c == '|';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("open_brace", [](char c) -> bool {
    return c == '{';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("close_brace", [](char c) -> bool {
    return c == '}';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("open_paren", [](char c) -> bool {
    return c == '(';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("close_paren", [](char c) -> bool {
    return c == ')';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("equals", [](char c) -> bool {
    return c == '=';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("any", [](char c) -> bool {
    return true;
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("comma", [](char c) -> bool {
    return c == ',';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("colon", [](char c) -> bool {
    return c == ':';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("hyphen", [](char c) -> bool {
    return c == '-';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("greater_chevron", [](char c) -> bool {
    return c == '>';
  });

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("exclamation", [](char c) -> bool {
    return c == '!';
  });
}

void addCharacterPatternToLookup(TokenType tokenType, std::string pattern) {
  auto parsedPattern = parseCharacterPattern(pattern);
  parsedPattern->setTokenType(tokenType);
  CharacterPatternLookup::getInstance()->addCharacterPattern(parsedPattern);
}

void loadCharacterPatterns() {
  addCharacterPatternToLookup(TokenType::Identifier, "(letter underscore)+");

  addCharacterPatternToLookup(TokenType::Decimal, "number+ full_stop number+");
  addCharacterPatternToLookup(TokenType::Integer, "number+");
  addCharacterPatternToLookup(TokenType::String, "double_quote` any* double_quote`");

  addCharacterPatternToLookup(TokenType::ForwardArrowOperator, "[hyphen greater_chevron]");
  addCharacterPatternToLookup(TokenType::StructureOperator, "[colon colon]");
  addCharacterPatternToLookup(TokenType::EllipsisOperator, "[full_stop full_stop full_stop]");

  addCharacterPatternToLookup(TokenType::SingleLineComment, "[forward_slash forward_slash]` any* new_line?`");
  addCharacterPatternToLookup(TokenType::MultiLineComment, "[forward_slash star]` any* [star forward_slash]`");

  addCharacterPatternToLookup(TokenType::LogicalConjunction, "[ampersand ampersand]");
  addCharacterPatternToLookup(TokenType::LogicalDisjunction,  "[pipe pipe]");
  addCharacterPatternToLookup(TokenType::LogicalNegation, "exclamation");

  addCharacterPatternToLookup(TokenType::LogicalEquality, "[equals equals]");

  addCharacterPatternToLookup(TokenType::BlockDelimiterOpen, "open_brace");
  addCharacterPatternToLookup(TokenType::BlockDelimiterClose, "close_brace");

  addCharacterPatternToLookup(TokenType::ParenthesisOpen, "open_paren");
  addCharacterPatternToLookup(TokenType::ParenthesisClose, "close_paren");

  addCharacterPatternToLookup(TokenType::Assignment, "equals");

  addCharacterPatternToLookup(TokenType::AddOperator, "plus");
  addCharacterPatternToLookup(TokenType::SubtractOperator, "hyphen");
  addCharacterPatternToLookup(TokenType::MultiplyOperator, "star");
  addCharacterPatternToLookup(TokenType::DivideOperator, "forward_slash");

  addCharacterPatternToLookup(TokenType::Comma, "comma");
}