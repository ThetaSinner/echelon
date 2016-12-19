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
    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("number", [] (char c) -> bool {
        return '0' <= c && c <= '9';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("letter", [] (char c) -> bool {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("underscore", [] (char c) -> bool {
        return c == '_';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("full_stop", [] (char c) -> bool {
        return c == '.';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("double_quote", [] (char c) -> bool {
        return c == '\"';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("forward_slash", [] (char c) -> bool {
        return c == '/';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("end_of_line", [] (char c) -> bool {
        return c == '\n';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("star", [] (char c) -> bool {
        return c == '*';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("ampersand", [] (char c) -> bool {
        return c == '&';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("pipe", [] (char c) -> bool {
        return c == '|';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("open_brace", [] (char c) -> bool {
        return c == '{';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("close_brace", [] (char c) -> bool {
        return c == '}';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("open_paren", [] (char c) -> bool {
        return c == '(';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("close_paren", [] (char c) -> bool {
        return c == ')';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("equals", [] (char c) -> bool {
        return c == '=';
    });

    CharacterMatcherLookup::getInstance() -> addCharacterMatcher("any", [] (char c) -> bool {
        return true;
    });
}

void loadCharacterPatterns() {
    std::string floatPattern = "number+ full_stop number+";
    auto floatCharacterPattern = parseCharacterPattern(floatPattern);
    floatCharacterPattern -> setTokenType(TokenTypeEnum::Float);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(floatCharacterPattern);

    std::string numberPattern = "number+";
    auto numberCharacterPattern = parseCharacterPattern(numberPattern);
    numberCharacterPattern -> setTokenType(TokenTypeEnum::Integer);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(numberCharacterPattern);

    std::string identifierPattern = "(letter underscore)+";
    auto identifierCharacterPattern = parseCharacterPattern(identifierPattern);
    identifierCharacterPattern -> setTokenType(TokenTypeEnum::Identifier);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(identifierCharacterPattern);

    std::string stringPattern = "double_quote any* double_quote";
    auto stringCharacterPattern = parseCharacterPattern(stringPattern);
    stringCharacterPattern -> setTokenType(TokenTypeEnum::String);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(stringCharacterPattern);

    std::string commentPattern = "forward_slash forward_slash any* end_of_line`";
    auto singleLineCommentCharacterPattern = parseCharacterPattern(commentPattern);
    singleLineCommentCharacterPattern -> setTokenType(TokenTypeEnum::SingleLineComment);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(singleLineCommentCharacterPattern);

    std::string multiLineCommentPattern = "[forward_slash star] any* [star forward_slash]";
    auto multiLineCommentCharacterPattern = parseCharacterPattern(multiLineCommentPattern);
    multiLineCommentCharacterPattern -> setTokenType(TokenTypeEnum::MultiLineComment);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(multiLineCommentCharacterPattern);

    std::string andOperator = "ampersand ampersand";
    auto andOperatorCharacterPattern = parseCharacterPattern(andOperator);
    andOperatorCharacterPattern -> setTokenType(TokenTypeEnum::AndOperator);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(andOperatorCharacterPattern);

    std::string orOperator = "pipe pipe";
    auto orOperatorCharacterPattern = parseCharacterPattern(orOperator);
    orOperatorCharacterPattern -> setTokenType(TokenTypeEnum::OrOperator);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(orOperatorCharacterPattern);

    std::string openBrace = "open_brace";
    auto blockDelimOpenCharacterPattern = parseCharacterPattern(openBrace);
    blockDelimOpenCharacterPattern -> setTokenType(TokenTypeEnum::BlockDelimO);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(blockDelimOpenCharacterPattern);

    std::string closeBrace = "close_brace";
    auto blockDelimCloseCharacterPattern = parseCharacterPattern(closeBrace);
    blockDelimCloseCharacterPattern -> setTokenType(TokenTypeEnum::BlockDelimC);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(blockDelimCloseCharacterPattern);

    std::string openParen = "open_paren";
    auto parenOpenCharacterPattern = parseCharacterPattern(openParen);
    parenOpenCharacterPattern -> setTokenType(TokenTypeEnum::ParenO);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(parenOpenCharacterPattern);

    std::string closeParen = "close_paren";
    auto parenCloseCharacterPattern = parseCharacterPattern(closeParen);
    parenCloseCharacterPattern -> setTokenType(TokenTypeEnum::ParenC);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(parenCloseCharacterPattern);

    std::string equality = "equals equals";
    auto equalityCharacterPattern = parseCharacterPattern(equality);
  equalityCharacterPattern -> setTokenType(TokenTypeEnum::Equality);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(equalityCharacterPattern);

    std::string equals = "equals";
    auto assignCharacterPattern = parseCharacterPattern(equals);
    assignCharacterPattern -> setTokenType(TokenTypeEnum::Assign);
    CharacterPatternLookup::getInstance() -> addCharacterPattern(assignCharacterPattern);
}