#ifndef CHARACTER_QUERY_INCLUDED
#define CHARACTER_QUERY_INCLUDED

#include <iostream>

class CharacterQuery {
public:
  static bool is_lower_case_letter(char in);
  static bool is_upper_case_letter(char in);
  static bool is_letter(char in);

  static bool is_block_delim_start(char in);
  static bool is_block_delim_stop(char in);
  static bool is_block_delim(char in);

  static bool is_paren_start(char in);
  static bool is_paren_stop(char in);
  static bool is_paren(char in);

  static bool is_sq_bracket_start(char in);
  static bool is_sq_bracket_stop(char in);
  static bool is_sq_bracket(char in);

  static bool is_underscore(char in);
  static bool is_whitespace(char in);
  static bool is_space(char in);
  static bool is_end_of_line(char in);
  static bool is_line_break(char in);
  static bool is_number(char in);
  static bool is_string_delim(char in);
};

#endif
