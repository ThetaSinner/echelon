#include <elm/parser/tokenizer.hpp>

#include <elm/optional.hpp>
#include <elm/parser/token.hpp>

#include "tokenizer-impl/character-query.hpp"
#include "tokenizer-impl/extractor-for-equality.hpp"
#include "tokenizer-impl/extractor-for-assignment.hpp"
#include "tokenizer-impl/extractor-for-line-comment.hpp"
#include "tokenizer-impl/extractor-for-multi-line-comment.hpp"
#include "tokenizer-impl/extractor-for-strings.hpp"

std::vector<Token> Tokenizer::tokenize(std::string in) {
  //std::cout << "Tokenizing string: " << in << std::endl;

  std::vector<Extractor*> extractors;
  extractors.push_back(new EqualityExtractor());
  extractors.push_back(new AssignmentExtractor());
  extractors.push_back(new LineCommentExtractor());
  extractors.push_back(new MultiLineCommentExtractor());
  extractors.push_back(new StringExtractor());

  std::vector<Token> tokens;

  long str_pos = 0;
  long in_len = in.size();
  while (str_pos < in_len) {
    char current_char = in.at(str_pos);

    if (CharacterQuery::is_whitespace(current_char)) {
      str_pos++;
      continue;
    }

    if (CharacterQuery::is_letter(current_char)) {
      int offset = 1;
      while (str_pos + offset < in_len) {
        char check_char = in.at(str_pos + offset);
        if (!CharacterQuery::is_letter(check_char) && !CharacterQuery::is_underscore(check_char) && !CharacterQuery::is_number(check_char)) {
          break;
        }

        offset++;
      }

      tokens.push_back(Token(in.substr(str_pos, offset), TokenTypeEnum::Identifier));
      str_pos += offset;
      continue;
    }

    if (CharacterQuery::is_number(current_char)) {
      int offset = 1;
      while (str_pos + offset < in_len) {
        char check_char = in.at(str_pos + offset);
        if (!CharacterQuery::is_number(check_char)) {
          break;
        }

        offset++;
      }

      tokens.push_back(Token(in.substr(str_pos, offset), TokenTypeEnum::Integer));
      str_pos += offset;
      continue;
    }

    if (CharacterQuery::is_block_delim(current_char)) {
      tokens.push_back(Token(in.substr(str_pos, 1), TokenTypeEnum::BlockDelim));
      str_pos++;
      continue;
    }

    if (CharacterQuery::is_paren(current_char)) {
      tokens.push_back(Token(in.substr(str_pos, 1), TokenTypeEnum::Paren));
      str_pos++;
      continue;
    }

    if (CharacterQuery::is_sq_bracket(current_char)) {
      tokens.push_back(Token(in.substr(str_pos, 1), TokenTypeEnum::SqBracket));
      str_pos++;
      continue;
    }

    std::vector<Token> extracted;

    ParseData parseData(in, str_pos);
    for (auto& extractor : extractors) {
      if (extractor->is_applicable(current_char)) {
        auto e = extractor->extract(parseData);
        if (e.isPresent()) {
          extracted.push_back(e.get());
        }
      }
    }

    if (extracted.size()) {
      Token selected = extracted.at(0);
      for (auto& ex : extracted) {
        if (ex.getData().size() > selected.getData().size()) {
          selected = ex;
        }
      }

      //std::cout << "Selected [" << selected << "]" << std::endl;
      tokens.push_back(selected);
      str_pos += selected.getData().size();
      continue;
    }


    std::string err = "Unhandled character + [" + std::string(1, current_char) + "] rest is [" + in.substr(str_pos);
    throw std::domain_error(err);
  }

  return tokens;
}
