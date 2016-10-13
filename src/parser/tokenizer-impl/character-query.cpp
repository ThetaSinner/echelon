#include "character-query.hpp"

bool CharacterQuery::is_lower_case_letter(char in) {
  return 97 <= in && in <= 122;
}

bool CharacterQuery::is_upper_case_letter(char in) {
  return 65 <= in && in <= 90;
}

bool CharacterQuery::is_letter(char in) {
  return is_lower_case_letter(in) || is_upper_case_letter(in);
}

bool CharacterQuery::is_underscore(char in) {
  return in == '_';
}

bool CharacterQuery::is_whitespace(char in) {
  return in == ' ' || in == '\n';
}

bool CharacterQuery::is_line_break(char in) {
  return in == '\n';
}

bool CharacterQuery::is_string_delim(char in) {
	return in == '"';
}

bool CharacterQuery::is_number(char in) {
  return 48 <= in && in <= 57;
}

bool CharacterQuery::is_block_delim_start(char in) {
  return in == '{';
}

bool CharacterQuery::is_block_delim_stop(char in) {
  return in == '}';
}

bool CharacterQuery::is_block_delim(char in) {
  return is_block_delim_start(in) || is_block_delim_stop(in);
}

bool CharacterQuery::is_paren_start(char in) {
  return in == '(';
}

bool CharacterQuery::is_paren_stop(char in) {
  return in == ')';
}

bool CharacterQuery::is_paren(char in) {
  return is_paren_start(in) || is_paren_stop(in);
}

bool CharacterQuery::is_sq_bracket_start(char in) {
  return in == '[';
}

bool CharacterQuery::is_sq_bracket_stop(char in) {
  return in == ']';
}

bool CharacterQuery::is_sq_bracket(char in) {
  return is_sq_bracket_start(in) || is_sq_bracket_stop(in);
}
