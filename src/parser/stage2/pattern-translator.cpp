#include <echelon/parser/stage2/pattern-translator.hpp>

#include <iostream>

#include <echelon/util/iterator-util.hpp>

bool is_letter(int c) {
  return (c >= 97) && (c <= 122);
}

bool is_identifier_char(int c) {
  return is_letter(c) || c == '_';
}

TokenPatternElement* PatternTranslator::readIdentifier(std::string::iterator& i, std::string& pattern) {
  if (is_letter(*i)) {
    auto it = i;
    while (is_identifier_char(*(++it))) {}

    TokenPatternElement *tokenPatternElement = new TokenPatternElement(std::string(i, it));

    safe_advance(i, it - i, pattern);

    return tokenPatternElement;
  }

  return nullptr;
}

TokenPattern* PatternTranslator::translate(std::string pattern) {
  TokenPattern *tokenPattern = new TokenPattern();

  for (auto i = pattern.begin(); i != pattern.end(); i++) {
    TokenPatternGroup *tokenPatternGroup = new TokenPatternGroup();

    if (*i == ' ') {
      continue;
    }
    else if (*i == '[') {
      // is optional.
      tokenPatternGroup -> setRepeatLowerBound(0);

      i++;
      while (true) {
        if (*i == ' ') {
          i++;
          continue;
        }

        auto ident = readIdentifier(i, pattern);

        if (ident == nullptr) {
          break;
        }

        tokenPatternGroup -> addElement(ident);
      }

      while (*i == ' ') {
        i++;
      }
      if (*i != ']') {
        std::cout << "(0042) syntax error.\n";
      }
      // consume the ']'
      safe_advance(i, 1, pattern);

      if (*i == '*') {
        tokenPatternGroup -> setRepeatUpperBound(-1);
        safe_advance(i, 1, pattern);
      }
    }
    else if (is_letter(*i)) {
      auto ident = readIdentifier(i, pattern);
      tokenPatternGroup -> addElement(ident);
    }
    else {
      std::cout << "(0041) syntax error." << *i << "\n";
      break;
    }

    tokenPattern -> addGroup(tokenPatternGroup);
    if (i == pattern.end()) {
      i--;
    }
  }

  return tokenPattern;
}
