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

  CharacterMatcherLookup::getInstance()->addCharacterMatcher("end_of_line", [](char c) -> bool {
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

void loadCharacterPatterns() {
  std::string floatPattern = "number+ full_stop number+";
  auto floatCharacterPattern = parseCharacterPattern(floatPattern);
  floatCharacterPattern->setTokenType(TokenType::Float);
  CharacterPatternLookup::getInstance()->addCharacterPattern(floatCharacterPattern);

  std::string numberPattern = "number+";
  auto numberCharacterPattern = parseCharacterPattern(numberPattern);
  numberCharacterPattern->setTokenType(TokenType::Integer);
  CharacterPatternLookup::getInstance()->addCharacterPattern(numberCharacterPattern);

  std::string identifierPattern = "(letter underscore)+";
  auto identifierCharacterPattern = parseCharacterPattern(identifierPattern);
  identifierCharacterPattern->setTokenType(TokenType::Identifier);
  CharacterPatternLookup::getInstance()->addCharacterPattern(identifierCharacterPattern);

  std::string stringPattern = "double_quote` any* double_quote`";
  auto stringCharacterPattern = parseCharacterPattern(stringPattern);
  stringCharacterPattern->setTokenType(TokenType::String);
  CharacterPatternLookup::getInstance()->addCharacterPattern(stringCharacterPattern);

  // TODO don't store the forward slashes.
  std::string commentPattern = "forward_slash forward_slash any* end_of_line`";
  auto singleLineCommentCharacterPattern = parseCharacterPattern(commentPattern);
  singleLineCommentCharacterPattern->setTokenType(TokenType::SingleLineComment);
  CharacterPatternLookup::getInstance()->addCharacterPattern(singleLineCommentCharacterPattern);

  std::string multiLineCommentPattern = "[forward_slash star] any* [star forward_slash]";
  auto multiLineCommentCharacterPattern = parseCharacterPattern(multiLineCommentPattern);
  multiLineCommentCharacterPattern->setTokenType(TokenType::MultiLineComment);
  CharacterPatternLookup::getInstance()->addCharacterPattern(multiLineCommentCharacterPattern);

  std::string andOperator = "ampersand ampersand";
  auto andOperatorCharacterPattern = parseCharacterPattern(andOperator);
  andOperatorCharacterPattern->setTokenType(TokenType::AndOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(andOperatorCharacterPattern);

  std::string orOperator = "pipe pipe";
  auto orOperatorCharacterPattern = parseCharacterPattern(orOperator);
  orOperatorCharacterPattern->setTokenType(TokenType::OrOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(orOperatorCharacterPattern);

  std::string openBrace = "open_brace";
  auto blockDelimOpenCharacterPattern = parseCharacterPattern(openBrace);
  blockDelimOpenCharacterPattern->setTokenType(TokenType::BlockDelimO);
  CharacterPatternLookup::getInstance()->addCharacterPattern(blockDelimOpenCharacterPattern);

  std::string closeBrace = "close_brace";
  auto blockDelimCloseCharacterPattern = parseCharacterPattern(closeBrace);
  blockDelimCloseCharacterPattern->setTokenType(TokenType::BlockDelimC);
  CharacterPatternLookup::getInstance()->addCharacterPattern(blockDelimCloseCharacterPattern);

  std::string openParen = "open_paren";
  auto parenOpenCharacterPattern = parseCharacterPattern(openParen);
  parenOpenCharacterPattern->setTokenType(TokenType::ParenO);
  CharacterPatternLookup::getInstance()->addCharacterPattern(parenOpenCharacterPattern);

  std::string closeParen = "close_paren";
  auto parenCloseCharacterPattern = parseCharacterPattern(closeParen);
  parenCloseCharacterPattern->setTokenType(TokenType::ParenC);
  CharacterPatternLookup::getInstance()->addCharacterPattern(parenCloseCharacterPattern);

  std::string equality = "equals equals";
  auto equalityCharacterPattern = parseCharacterPattern(equality);
  equalityCharacterPattern->setTokenType(TokenType::Equality);
  CharacterPatternLookup::getInstance()->addCharacterPattern(equalityCharacterPattern);

  std::string equals = "equals";
  auto assignCharacterPattern = parseCharacterPattern(equals);
  assignCharacterPattern->setTokenType(TokenType::Assign);
  CharacterPatternLookup::getInstance()->addCharacterPattern(assignCharacterPattern);

  std::string comma = "comma";
  auto commaCharacterPattern = parseCharacterPattern(comma);
  commaCharacterPattern->setTokenType(TokenType::CommaOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(commaCharacterPattern);

  std::string addOperator = "plus";
  auto addOperatorCharacterPattern = parseCharacterPattern(addOperator);
  addOperatorCharacterPattern->setTokenType(TokenType::OperatorAdd);
  CharacterPatternLookup::getInstance()->addCharacterPattern(addOperatorCharacterPattern);

  std::string multiplyOperator = "star";
  auto multiplyOperatorPattern = parseCharacterPattern(multiplyOperator);
  multiplyOperatorPattern->setTokenType(TokenType::OperatorMultiply);
  CharacterPatternLookup::getInstance()->addCharacterPattern(multiplyOperatorPattern);

  std::string structureOperator = "colon colon";
  auto structureOperatorPattern = parseCharacterPattern(structureOperator);
  structureOperatorPattern->setTokenType(TokenType::StructureOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(structureOperatorPattern);

  std::string forwardArrowOperator = "hyphen greater_chevron";
  auto forwardArrowOperatorPattern = parseCharacterPattern(forwardArrowOperator);
  forwardArrowOperatorPattern->setTokenType(TokenType::ForwardArrowOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(forwardArrowOperatorPattern);

  std::string ellipsisOperator = "full_stop full_stop full_stop";
  auto ellipsisOperatorPattern = parseCharacterPattern(ellipsisOperator);
  ellipsisOperatorPattern->setTokenType(TokenType::EllipsisOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(ellipsisOperatorPattern);

  std::string notOperator = "exclamation";
  auto notOperatorPattern = parseCharacterPattern(notOperator);
  notOperatorPattern->setTokenType(TokenType::NotOperator);
  CharacterPatternLookup::getInstance()->addCharacterPattern(notOperatorPattern);
}