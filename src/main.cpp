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

bool isIdent(char c) {
  return (97 <= c && c <= 122) || (48 <= c && c <= 57) || c == '_';
}


class CharacterPatternElement {
  std::string data;

  bool repeatable = false;

  CharacterMatcher matcher;

  bool useLookahead = false;

public:
  CharacterPatternElement(std::string data) : data(data) {
    matcher = CharacterMatcherLookup::getInstance() -> getMatcher(data);

    if (data == "any") {
      useLookahead = true;
    }
  }

  std::string getData() {
    return data;
  }

  CharacterMatcher getMatcher() {
    return matcher;
  }

  bool isUseLookahead() {
    return useLookahead;
  }

  void setRepeatable(bool repeatable) {
    this -> repeatable = repeatable;
  }

  bool isRepeatable() {
    return repeatable;
  }
};

enum class CharacterPatternGroupType {
    Union,
    Sequence,
};

class CharacterPatternGroup {
  std::list<CharacterPatternElement*> elements;

  bool repeatable = false;

  CharacterPatternGroupType type;

public:
  void addElement(CharacterPatternElement* cpe) {
    elements.push_back(cpe);
  }

  std::list<CharacterPatternElement*>* getElements() {
    return &elements;
  }

  void setRepeatable(bool repeatable) {
    this -> repeatable = repeatable;
  }

  bool isRepeatable() {
    return repeatable;
  }

  void setType(CharacterPatternGroupType type) {
    this -> type = type;
  }

  CharacterPatternGroupType getType() {
    return type;
  }
};

class CharacterPattern {
  std::list<CharacterPatternGroup*> groups;
public:
  void addGroup(CharacterPatternGroup* cpg) {
    groups.push_back(cpg);
  }

  std::list<CharacterPatternGroup*>* getGroups() {
    return &groups;
  }
};

bool matchUnionGroup(std::list<CharacterPatternGroup *> *groups, std::list<CharacterPatternGroup *>::iterator &group, std::string::iterator &ig);
bool matchSequenceGroup(std::list<CharacterPatternGroup *> *groups, std::list<CharacterPatternGroup *>::iterator &group, std::string::iterator &ig);

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig);

CharacterPattern* parseCharacterPattern(std::string pattern) {
  CharacterPattern *characterPattern = new CharacterPattern();

  int strPos = 0;
  int strLen = pattern.size();
  while (strPos < strLen) {
    auto currentChar = pattern[strPos];

    if (currentChar == '(') {
      int subStrPos = strPos + 1;

      CharacterPatternGroup *group = new CharacterPatternGroup();
      group -> setType(CharacterPatternGroupType::Union);
      while (subStrPos < strLen && pattern[subStrPos] != ')') {
        int start = subStrPos;
        while (isIdent(pattern[subStrPos])) {
          subStrPos++;
        }

        group -> addElement(new CharacterPatternElement(pattern.substr(start, subStrPos - start)));
        if (pattern[subStrPos] == ' ') {
          subStrPos++;
        }
      }

      if (pattern[subStrPos] == ')') {
        subStrPos++;
      }

      if (pattern[subStrPos] == '*') {
        group -> setRepeatable(true);
        subStrPos++;
      }

      characterPattern -> addGroup(group);
      strPos = subStrPos;
      continue;
    }

    if (currentChar == '[') {
      int subStrPos = strPos + 1;

      CharacterPatternGroup *group = new CharacterPatternGroup();
      group -> setType(CharacterPatternGroupType::Sequence);
      while (subStrPos < strLen && pattern[subStrPos] != ']') {
        int start = subStrPos;
        while (isIdent(pattern[subStrPos])) {
          subStrPos++;
        }

        group -> addElement(new CharacterPatternElement(pattern.substr(start, subStrPos - start)));
        if (pattern[subStrPos] == ' ') {
          subStrPos++;
        }
      }

      if (pattern[subStrPos] == ']') {
        subStrPos++;
      }

      characterPattern -> addGroup(group);
      strPos = subStrPos;
      continue;
    }

    if (isIdent(pattern[strPos])) {
      int start = strPos;
      int subStrPos = strPos;
      while (isIdent(pattern[subStrPos])) {
        subStrPos++;
      }

      CharacterPatternElement *cpe = new CharacterPatternElement(pattern.substr(start, subStrPos - start));

      CharacterPatternGroup *cpg = new CharacterPatternGroup();
      cpg -> addElement(cpe);

      characterPattern -> addGroup(cpg);

      if (pattern[subStrPos] == '*') {
        cpe -> setRepeatable(true);
        subStrPos++;
      }
      strPos = subStrPos;
      continue;
    }

    strPos++;
  }

  return characterPattern;
}

void tokenize(std::string input, std::list<CharacterPattern*> patternList) {

  auto i = input.begin();

  while (i != input.end()) {
    if (*i == ' ' || *i == '\n') {
      i++;
      continue;
    }

    for (auto pattern : patternList) {

      auto ip = i;
      bool patternMatches = true;
      int groupMatchCount = 0;
      auto groups = pattern -> getGroups();
      for (auto group = groups -> begin(); group != groups -> end(); group++) {

        auto ig = ip;
        bool groupMatches;
        if ((*group) -> getType() == CharacterPatternGroupType::Union) {
          groupMatches = matchUnionGroup(groups, group, ig);
        }
        else {
          groupMatches = matchSequenceGroup(groups, group, ig);
        }

        if (groupMatches) {
          std::advance(ip, ig - ip);

          if ((*group) -> isRepeatable()) {
            groupMatchCount++;
            group--;
          }
          else {
            groupMatchCount = 0;
          }
        }
        else {
          if (groupMatchCount == 0) {
            patternMatches = false;
          }

          break;
        }
      }

      if (patternMatches) {
        std::cout << input.substr(i - input.begin(), ip - i) << std::endl;
        std::advance(i, ip - i);
      }

      if (i == input.end()) {
        break;
      }
    }
  }

}

bool matchUnionGroup(std::list<CharacterPatternGroup *> *groups, std::list<CharacterPatternGroup *>::iterator &group, std::string::iterator &ig) {
  bool groupMatches = false;

  auto elements = (*group) -> getElements();
  for (auto element = elements -> begin(); element != elements -> end(); element++) {
    auto matcher = (*element) -> getMatcher();

    if (matcher(*ig) && !matchLookahead(element, group, groups, ig)) {
      ig++;
      groupMatches = true;

      if ((*element) -> isRepeatable()) {
        while (matcher(*ig) && !matchLookahead(element, group, groups, ig)) {
          ig++;
        }
      }

      break;
    }
  }

  return groupMatches;
}

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig) {

  bool nextGroupMatches = false;
  if ((*element) -> isUseLookahead()) {
    auto next_group = std::next(group, 1);
    std::string::iterator ig_copy = ig;
    if ((*next_group) -> getType() == CharacterPatternGroupType::Union) {
      nextGroupMatches = matchUnionGroup(groups, next_group, ig_copy);
    }
    else {
      nextGroupMatches = matchSequenceGroup(groups, next_group, ig_copy);
    }
  }

  return nextGroupMatches;
}

bool matchSequenceGroup(std::list<CharacterPatternGroup *> *groups, std::list<CharacterPatternGroup *>::iterator &group, std::string::iterator &ig) {
  bool groupMatches = true;

  auto elements = (*group) -> getElements();
  for (auto element = elements -> begin(); element != elements -> end(); element++) {
    auto matcher = (*element) -> getMatcher();

    if (matcher(*ig)) {
      ig++;
    }
    else {
      groupMatches = false;
      break;
    }
  }

  return groupMatches;
}

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

  std::string numberPattern = "number*";
  std::string identifierPattern = "(letter underscore)*";
  std::string floatPattern = "number* full_stop number*";
  std::string stringPattern = "double_quote any* double_quote";
  std::string commentPattern = "forward_slash forward_slash any* end_of_line";
  std::string multiLineCommentPattern = "[forward_slash star] any* [star forward_slash]";
  std::string andOperator = "ampersand ampersand";
  std::string orOperator = "pipe pipe";
  std::string openBrace = "open_brace";
  std::string closeBrace = "close_brace";
  std::string openParen = "open_paren";
  std::string closeParen = "close_paren";
  std::string equals = "equals";

  std::string coverageString = "test1 test2* (test3) (test4)* (test5 test6)*";

  std::list<CharacterPattern*> patternList;
  patternList.push_back(parseCharacterPattern(floatPattern));
  patternList.push_back(parseCharacterPattern(numberPattern));
  patternList.push_back(parseCharacterPattern(identifierPattern));
  patternList.push_back(parseCharacterPattern(stringPattern));
  patternList.push_back(parseCharacterPattern(commentPattern));
  patternList.push_back(parseCharacterPattern(multiLineCommentPattern));
  patternList.push_back(parseCharacterPattern(andOperator));
  patternList.push_back(parseCharacterPattern(orOperator));
  patternList.push_back(parseCharacterPattern(openBrace));
  patternList.push_back(parseCharacterPattern(closeBrace));
  patternList.push_back(parseCharacterPattern(openParen));
  patternList.push_back(parseCharacterPattern(closeParen));
  patternList.push_back(parseCharacterPattern(equals));

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
