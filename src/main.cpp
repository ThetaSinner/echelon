#include <iostream>
#include <map>
#include <list>
#include <fstream>

#include <echelon/parser/parser-data/parser-data-load.hpp>
#include <echelon/parser/stage2/pattern-translator.hpp>
#include <echelon/parser/stage2/parser.hpp>
#include <echelon/util/ast-to-graphviz.hpp>
#include <echelon/transform/type-deduction-engine.hpp>
#include <echelon/util/stream-dump.hpp>
#include <echelon/code-generation/code-generator.hpp>
#include <echelon/parser/stage1/character-matcher-lookup.hpp>
#include <echelon/parser/stage1/character-pattern-parser.hpp>
#include <echelon/parser/stage1/tokenizer.hpp>
#include <echelon/parser/token-type-enum.hpp>

int main(int argc, char** args) {
  #ifdef ECHELON_DEBUG
  std::cout << "This is a debug build.\n";
  #else
  std::cout << "This is a release build.\n";
  #endif

  std::string exampleTokenString = "abc { } {{ 4321 432.1 () (( <> << [] [[ || | && & # \"\" ! '' ' * ^ $ , .";

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

  std::list<CharacterPattern*> patternList;

  std::string floatPattern = "number* full_stop number*";
  auto floatCharacterPattern = parseCharacterPattern(floatPattern);
  floatCharacterPattern -> setTokenType(TokenTypeEnum::Float);
  patternList.push_back(floatCharacterPattern);

  std::string numberPattern = "number*";
  auto numberCharacterPattern = parseCharacterPattern(numberPattern);
  numberCharacterPattern -> setTokenType(TokenTypeEnum::Integer);
  patternList.push_back(numberCharacterPattern);

  std::string identifierPattern = "(letter underscore)*";
  auto identifierCharacterPattern = parseCharacterPattern(identifierPattern);
  identifierCharacterPattern -> setTokenType(TokenTypeEnum::Identifier);
  patternList.push_back(identifierCharacterPattern);

  std::string stringPattern = "double_quote any* double_quote";
  auto stringCharacterPattern = parseCharacterPattern(stringPattern);
  stringCharacterPattern -> setTokenType(TokenTypeEnum::String);
  patternList.push_back(stringCharacterPattern);

  std::string commentPattern = "forward_slash forward_slash any* end_of_line";
  auto singleLineCommentCharacterPattern = parseCharacterPattern(commentPattern);
  singleLineCommentCharacterPattern -> setTokenType(TokenTypeEnum::SingleLineComment);
  patternList.push_back(singleLineCommentCharacterPattern);

  std::string multiLineCommentPattern = "[forward_slash star] any* [star forward_slash]";
  auto multiLineCommentCharacterPattern = parseCharacterPattern(multiLineCommentPattern);
  multiLineCommentCharacterPattern -> setTokenType(TokenTypeEnum::MultiLineComment);
  patternList.push_back(multiLineCommentCharacterPattern);

  std::string andOperator = "ampersand ampersand";
  auto andOperatorCharacterPattern = parseCharacterPattern(andOperator);
  andOperatorCharacterPattern -> setTokenType(TokenTypeEnum::AndOperator);
  patternList.push_back(andOperatorCharacterPattern);

  std::string orOperator = "pipe pipe";
  auto orOperatorCharacterPattern = parseCharacterPattern(orOperator);
  orOperatorCharacterPattern -> setTokenType(TokenTypeEnum::OrOperator);
  patternList.push_back(orOperatorCharacterPattern);

  std::string openBrace = "open_brace";
  auto blockDelimOpenCharacterPattern = parseCharacterPattern(openBrace);
  blockDelimOpenCharacterPattern -> setTokenType(TokenTypeEnum::BlockDelimO);
  patternList.push_back(blockDelimOpenCharacterPattern);

  std::string closeBrace = "close_brace";
  auto blockDelimCloseCharacterPattern = parseCharacterPattern(closeBrace);
  blockDelimCloseCharacterPattern -> setTokenType(TokenTypeEnum::BlockDelimC);
  patternList.push_back(blockDelimCloseCharacterPattern);

  std::string openParen = "open_paren";
  auto parenOpenCharacterPattern = parseCharacterPattern(openParen);
  parenOpenCharacterPattern -> setTokenType(TokenTypeEnum::ParenO);
  patternList.push_back(parenOpenCharacterPattern);

  std::string closeParen = "close_paren";
  auto parenCloseCharacterPattern = parseCharacterPattern(closeParen);
  parenCloseCharacterPattern -> setTokenType(TokenTypeEnum::ParenC);
  patternList.push_back(parenCloseCharacterPattern);

  std::string equals = "equals";
  auto assignCharacterPattern = parseCharacterPattern(equals);
  assignCharacterPattern -> setTokenType(TokenTypeEnum::Assign);
  patternList.push_back(assignCharacterPattern);

  tokenize("9011", patternList);
  tokenize("as_df", patternList);
  tokenize("asdf fdas", patternList);
  tokenize("234.45", patternList);
  tokenize("\"happy elf\"", patternList);
  tokenize("// healthy comment\n", patternList);
  tokenize("/* some \n multi \n line \n comment */", patternList);
  tokenize("&&", patternList);
  tokenize("||", patternList);
  tokenize("{", patternList);
  tokenize("}", patternList);
  tokenize("if (true || false) {\n  integer x = 5\n}", patternList);

  return 0;








  loadParserData();

  std::cout << "Parser data loaded." << std::endl;

  std::list<Token*> program;
  program.push_back(new Token("if", TokenTypeEnum::Identifier));
  program.push_back(new Token("(", TokenTypeEnum::ParenO));
  program.push_back(new Token("1", TokenTypeEnum::Integer));
  program.push_back(new Token("==", TokenTypeEnum::Equality));
  program.push_back(new Token("2", TokenTypeEnum::Integer));
  program.push_back(new Token(")", TokenTypeEnum::ParenC));
  program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program.push_back(new Token("string", TokenTypeEnum::Identifier));
  program.push_back(new Token("cond_var", TokenTypeEnum::Identifier));
  program.push_back(new Token("=", TokenTypeEnum::Assign));
  program.push_back(new Token("string_one", TokenTypeEnum::String));
  program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));
  program.push_back(new Token("else", TokenTypeEnum::Identifier));
  program.push_back(new Token("{", TokenTypeEnum::BlockDelimO));
  program.push_back(new Token("string", TokenTypeEnum::Identifier));
  program.push_back(new Token("alt_cond_var", TokenTypeEnum::Identifier));
  program.push_back(new Token("=", TokenTypeEnum::Assign));
  program.push_back(new Token("string_two", TokenTypeEnum::String));
  program.push_back(new Token("}", TokenTypeEnum::BlockDelimC));

  Parser2 parser;
  auto ast = parser.parse(program);

  std::ofstream out("test.gv");
  std::string gv = toGraphviz(ast);
  std::cout << gv << std::endl;
  // dot test.gv -Tjpeg > img.jpeg
  out << gv;
  out.close();

  // Need a good data scructure which has all the declared data, types and functions.
  // It needs to be such that it is simple to find out whether something is defined in the current context or parent context
  // for example. Maybe it will be a complementary structure to the existing tree.

  std::list<Token*> program2;
  program2.push_back(new Token("my_str_var", TokenTypeEnum::Identifier));
  program2.push_back(new Token("=", TokenTypeEnum::Assign));
  program2.push_back(new Token("str data", TokenTypeEnum::String));

  auto str_no_type_ast = parser.parse(program2);

  TypeDeductionEngine typeDeductionEngine;
  typeDeductionEngine.deduceTypes(str_no_type_ast);

  stream_dump(std::cout, str_no_type_ast);

  CodeGenerator codeGenerator;
  std::cout << codeGenerator.generate(str_no_type_ast) << std::endl;

  std::cout << std::endl << "Program will exit normally.";
  std::cout << std::endl;
  return 0;
}
