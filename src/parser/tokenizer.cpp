#include "elm/parser/tokenizer.hpp"

#include "elm/optional.hpp"

void emit() {
  std::cout << "emitting\n";
}

class ParseData {
private:
  std::string parse_str;
  int parse_pos = 0;
  int parse_len = 0;
public:
  ParseData(std::string parse_str, int parse_pos) {
    this->parse_str = parse_str;
    this->parse_pos = parse_pos;
    this->parse_len = parse_str.size();
  }

  std::string getParseStr() {
    return parse_str;
  }

  int getParsePos() {
    return parse_pos;
  }

  void setParsePos(int parse_pos) {
    this->parse_pos = parse_pos;
  }

  int getParseLen() {
    return parse_len;
  }

  bool hasNext() {
    return getParsePos() + 1 < getParseLen();
  }

  char getCurrent() {
    return parse_str.at(parse_pos);
  }

  char getNext() {
    return parse_str.at(parse_pos + 1);
  }
};

class Extractor {
public:
  virtual bool is_applicable(char apply_char) = 0;
  virtual Optional<std::string> extract(ParseData parseData) = 0;
};

class EqualityExtractor : public Extractor {
public:
  bool is_applicable(char apply_char) override {
    return apply_char == '=';
  };

  Optional<std::string> extract(ParseData parseData) override {
    if (parseData.hasNext() && parseData.getNext() == '=') {
      return Optional<std::string>::of(parseData.getParseStr().substr(parseData.getParsePos(), 2));
    }

    return Optional<std::string>::absent();
  };
};

class AssignmentExtractor : public Extractor {
public:
  bool is_applicable(char apply_char) override {
    return apply_char == '=';
  };

  Optional<std::string> extract(ParseData parseData) override {
    return Optional<std::string>::of(std::string(1, parseData.getCurrent()));
  };
};

std::vector<std::string> Tokenizer::tokenize(std::string in) {
  std::cout << "Tokenizing string: " << in << std::endl;

  std::vector<Extractor*> extractors;
  extractors.push_back(new EqualityExtractor());
  extractors.push_back(new AssignmentExtractor());

  std::vector<std::string> tokens;

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

      tokens.push_back(in.substr(str_pos, offset));
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

      tokens.push_back(in.substr(str_pos, offset));
      str_pos += offset;
      continue;
    }

    if (CharacterQuery::is_block_delim(current_char)) {
      tokens.push_back(in.substr(str_pos, 1));
      str_pos++;
      continue;
    }

    if (CharacterQuery::is_paren(current_char)) {
      tokens.push_back(in.substr(str_pos, 1));
      str_pos++;
      continue;
    }

    if (CharacterQuery::is_sq_bracket(current_char)) {
      tokens.push_back(in.substr(str_pos, 1));
      str_pos++;
      continue;
    }

    std::vector<std::string> extracted;

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
      std::string selected = extracted.at(0);
      for (auto& ex : extracted) {
        if (ex.size() > selected.size()) {
          selected = ex;
        }
      }

      std::cout << "Selected " << selected << std::endl;
      tokens.push_back(selected);
      str_pos += selected.size();
      continue;
    }


    std::string err = "Unhandled character + [" + std::string(1, current_char) + "]";
    throw std::domain_error(err);
  }

  int token_num = 0;
  for (auto &i : tokens) {
    std::cout << token_num << " " << i << "\n";
    token_num++;
  }

  return tokens;
}
