#include "elm/parser/parser.hpp"

bool is_lower_case_letter(char in) {
  return 97 <= in && in <= 122;
}

bool is_upper_case_letter(char in) {
  return 65 <= in && in <= 90;
}

bool is_letter(char in) {
  return is_lower_case_letter(in) || is_upper_case_letter(in);
}

bool is_underscore(char in) {
  return in == '_';
}

bool is_whitespace(char in) {
  return in == ' ' || in == '\n';
}

bool is_number(char in) {
  return 48 <= in && in <= 57;
}

bool is_block_delim_start(char in) {
  return in == '{';
}

bool is_block_delim_stop(char in) {
  return in == '}';
}

bool is_block_delim(char in) {
  return is_block_delim_start(in) || is_block_delim_stop(in);
}

bool is_paren_start(char in) {
  return in == '(';
}

bool is_paren_stop(char in) {
  return in == ')';
}

bool is_paren(char in) {
  return is_paren_start(in) || is_paren_stop(in);
}

bool is_sq_bracket_start(char in) {
  return in == '[';
}

bool is_sq_bracket_stop(char in) {
  return in == ']';
}

bool is_sq_bracket(char in) {
  return is_sq_bracket_start(in) || is_sq_bracket_stop(in);
}

std::vector<std::string> Parser::parse(std::string in) {
  std::cout << "Parsing string: " << in << std::endl;

  std::vector<std::string> tokens;

  long str_pos = 0;
  long in_len = in.size();
  while (str_pos < in_len) {
    char current_char = in.at(str_pos);

    if (is_whitespace(current_char)) {
      str_pos++;
      continue;
    }

    if (is_letter(current_char)) {
      int offset = 1;
      while (str_pos + offset < in_len) {
        char check_char = in.at(str_pos + offset);
        if (!is_letter(check_char) && !is_underscore(check_char) && !is_number(check_char)) {
          break;
        }

        offset++;
      }

      tokens.push_back(in.substr(str_pos, offset));
      str_pos += offset;
      continue;
    }

    if (is_number(current_char)) {
      int offset = 1;
      while (str_pos + offset < in_len) {
        char check_char = in.at(str_pos + offset);
        if (!is_number(check_char)) {
          break;
        }

        offset++;
      }

      tokens.push_back(in.substr(str_pos, offset));
      str_pos += offset;
      continue;
    }

    if (is_block_delim(current_char)) {
      tokens.push_back(in.substr(str_pos, 1));
      str_pos++;
    }

    if (is_paren(current_char)) {
      tokens.push_back(in.substr(str_pos, 1));
      str_pos++;
    }

    if (is_sq_bracket(current_char)) {
      tokens.push_back(in.substr(str_pos, 1));
      str_pos++;
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
